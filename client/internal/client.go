package internal

import (
	"io"
	"log"
	"net/http"
)

func Msg() (string, error) {
	url := "http://localhost:8080"

	resp, err := http.Get(url)
	if err != nil {
		log.Fatalf("Error making GET request: %v", err)
	}
	defer resp.Body.Close()

	// check the response status
	if resp.StatusCode != http.StatusOK {
		log.Fatalf("Received non-OK HTTP status: %s", resp.Status)
	}

	// reads the response body
	body, err := io.ReadAll(resp.Body)
	if err != nil {
		log.Fatalf("Error reading response body: %v", err)
	}

	// fmt.Printf("Response from %s:\n%s\n", url, string(body))
	return string(body), nil
}
