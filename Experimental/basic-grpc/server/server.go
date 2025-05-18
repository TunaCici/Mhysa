package main

import (
	"context"
	"encoding/json"
	"fmt"
	"log"
	"math/rand/v2"
	"net"
	"net/http"
	"net/url"
	"time"

	pb "github.com/TunaCici/Mhysa/Experimental/basic-grpc/proto"
	"google.golang.org/grpc"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type NominatimResult struct {
	Lat string `json:"lat"`
	Lon string `json:"lon"`
}

type OpenMeteoResponse struct {
	CurrentWeather struct {
		Temperature float64 `json:"temperature"`
	} `json:"current_weather"`
}

type server struct {
	pb.UnimplementedGeneralInfoServer
}

func (s *server) GetWeather(ctx context.Context, in *pb.WeatherRequest) (*pb.WeatherReply, error) {
	log.Println("GetWeather: ", in.City)

	// 1. Contact Nominatum servers to get 'city' coords
	nomURL := fmt.Sprintf("https://nominatim.openstreetmap.org/search?q=%s&format=json&limit=1", url.QueryEscape(in.City))
	req, _ := http.NewRequest("GET", nomURL, nil)
	req.Header.Set("User-Agent", "go-weather-cli")

	log.Println("GET", "nominatim.openstreetmap.org", in.City)
	res, err := http.DefaultClient.Do(req)
	if err != nil {
		return &pb.WeatherReply{}, nil
	}
	defer res.Body.Close()

	var results []NominatimResult
	if err := json.NewDecoder(res.Body).Decode(&results); err != nil {
		return &pb.WeatherReply{}, nil
	}

	if len(results) == 0 {
		log.Println("no results found for city: ", in.City)
		return &pb.WeatherReply{}, nil
	}

	// 2. Contact Open-Meteo servers to get degree in celcius
	wthrURL := fmt.Sprintf("https://api.open-meteo.com/v1/forecast?latitude=%s&longitude=%s&current_weather=true", results[0].Lat, results[0].Lon)

	log.Println("GET", "api.open-meteo.com", results[0].Lat, results[0].Lon)
	resp2, err := http.Get(wthrURL)
	if err != nil {
		return &pb.WeatherReply{}, nil
	}
	defer resp2.Body.Close()

	var weather struct {
		CurrentWeather struct {
			Temperature float64 `json:"temperature"`
		} `json:"current_weather"`
	}
	json.NewDecoder(resp2.Body).Decode(&weather)

	out := pb.WeatherReply{
		City:        in.City,
		Temperature: fmt.Sprintf("%.1f °C", weather.CurrentWeather.Temperature),
	}

	return &out, nil
}

func (s *server) GetStock(in *pb.StockRequest, stream grpc.ServerStreamingServer[pb.StockReply]) error {
	log.Println("GetStock:", in.Symbol)

	// TODO: 1. Find a WebSocket API to get basic stock market data
	for {
		out := pb.StockReply{
			Symbol:    in.Symbol,
			Price:     130.0 + (rand.Float32()*2 - 1),
			Timestamp: timestamppb.Now(),
		}

		err := stream.Send(&out)
		if err != nil {
			return err
		}

		time.Sleep(500 * time.Millisecond)
	}

	return nil
}

func main() {
	log.Println("Starting the server")

	list, err := net.Listen("tcp", "localhost:50051")
	if err != nil {
		log.Fatalln("error listening to localhost:50051: %v", err)
	}

	var opts []grpc.ServerOption

	grpcServer := grpc.NewServer(opts...)
	pb.RegisterGeneralInfoServer(grpcServer, &server{})
	grpcServer.Serve(list)
}
