package main

import (
	"context"
	"fmt"
	"io"
	"os"
	"text/tabwriter"
	"time"

	pb "github.com/TunaCici/Mhysa/Experimental/basic-grpc/proto"
	"google.golang.org/grpc"
	"google.golang.org/grpc/credentials/insecure"
)

const (
	City        = "Manchester"
	StockSymbol = "NVDA"
)

func main() {
	// 0. Pretty output
	w := tabwriter.NewWriter(os.Stdout, 0, 4, 2, ' ', 0)

	// 1. Connection
	conn, err := grpc.NewClient("localhost:50051", grpc.WithTransportCredentials(insecure.NewCredentials()))
	if err != nil {
		fmt.Fprintln(os.Stderr, "failed to connect to localhost:50051: ", err)
	}
	defer conn.Close()
	c := pb.NewGeneralInfoClient(conn)

	// 2. RPC to GetWeather
	ctx, cancel := context.WithTimeout(context.Background(), 3*time.Second)
	defer cancel()

	wthr, err := c.GetWeather(ctx, &pb.WeatherRequest{City: City})
	if err != nil {
		fmt.Fprintln(os.Stderr, "failed to GetWeather: ", err)
	}

	fmt.Println(wthr)

	// 3. Server-Stream to GetStock
	ctx, cancel = context.WithCancel(context.Background())
	defer cancel()

	stream, err := c.GetStock(ctx, &pb.StockRequest{Symbol: StockSymbol})
	if err != nil {
		fmt.Fprintln(os.Stderr, "failed to stream GetStock: ", err)
	}

	// 4. Display info forever (and as fast as possible)
	for {
		stck, err := stream.Recv()
		if err == io.EOF {
			fmt.Println("stream to GetStock ended")
		}
		if err != nil {
			fmt.Fprintln(os.Stderr, "failed to Recv: ", err)
		}

		fmt.Fprintf(w, "\033[H\033[2J")
		fmt.Fprintf(w, "--------------------------------\n")
		fmt.Fprintf(w, "%s (%s)\n", wthr.City, wthr.Temperature)
		fmt.Fprintf(w, "--------------------------------\n")
		fmt.Fprintf(w, "Symbol\tPrice\tLast updated\n")
		fmt.Fprintf(w, "%s\t%.2f\t%s\n", stck.Symbol, stck.Price, stck.Timestamp.AsTime().Format("15:04:05"))
		fmt.Fprintf(w, "--------------------------------\n")
		w.Flush()
	}
}
