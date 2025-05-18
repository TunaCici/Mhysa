package main

import (
	"context"
	"log"
	"time"

	pb "github.com/TunaCici/Mhysa/Experimental/basic-grpc/proto"
	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"
)

func main() {
	conn, err := grpc.NewClient("localhost:50051", grpc.WithTransportCredentials(insecure.NewCredentials()))
	if err != nil {
		log.Fatalln("failed to connect to localhost:50051: ", err)
	}
	defer conn.Close()
	c := pb.NewGeneralInfoClient(conn)

	ctx, cancel := context.WithTimeout(context.Background(), 3*time.Second)
	defer cancel()

	resp, err := c.GetWeather(ctx, &pb.WeatherRequest{City: "Los Angeles"})
	if err != nil {
		log.Fatalln("failed to GetWeather: ", err)
	}
	log.Println(resp)

	stream, err := c.GetStock(ctx, &pb.StockRequest{Symbol: "ARM"})
	if err != nil {
		log.Fatalln("failed to GetStock: ", err)
	}

	for {
		resp, err := stream.Recv()
		if err != nil {
			log.Fatalln("failed to Recv: ", err)
		}

		log.Println(resp)
	}
}
