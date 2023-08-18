package main

import (
    "fmt"
    "log"
    "time"
    "math"
    "net/http"
)

var lock int

func main() {
     lock = 0
     http.HandleFunc("/metrics", func(w http.ResponseWriter, r *http.Request){
	t := math.Sin(float64(time.Now().Unix())/60) 
	log.Println("Fetched metrics")
	fmt.Fprintf(w, "light_measurement %f\n", t)
	fmt.Fprintf(w, "lock %d\n",lock)
    })

    http.HandleFunc("/lock", func(w http.ResponseWriter, r *http.Request){
    	lock = 1
    })

    http.HandleFunc("/unlock", func(w http.ResponseWriter, r *http.Request){
  	lock = 0
    })


    fmt.Printf("Starting server at port 8080\n")
    if err := http.ListenAndServe(":8080", nil); err != nil {
        log.Fatal(err)
    }
}
