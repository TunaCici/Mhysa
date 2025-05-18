package main

import (
	"context"
	"log"
	"net"
	"time"

	pb "github.com/TunaCici/Mhysa/Experimental/basic-grpc/proto"
	"google.golang.org/grpc"
	"google.golang.org/protobuf/types/known/timestamppb"
)

type server struct {
	pb.UnimplementedGeneralInfoServer
}

func (s *server) GetWeather(ctx context.Context, in *pb.WeatherRequest) (*pb.WeatherReply, error) {
	log.Println("GetWeather: ", in.City)

	out := pb.WeatherReply{
		City:        in.City,
		Temperature: "30 Celcius",
	}

	return &out, nil
}

func (s *server) GetStock(in *pb.StockRequest, stream grpc.ServerStreamingServer[pb.StockReply]) error {
	log.Println("GetStock: ", in.Symbol)

	for {
		out := pb.StockReply{
			Symbol:    in.Symbol,
			Price:     56.13,
			Timestamp: timestamppb.Now(),
		}

		err := stream.Send(&out)
		if err != nil {
			return err
		}

		time.Sleep(1000 * time.Millisecond)
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
