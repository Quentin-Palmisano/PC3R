package main

import (
	"bufio"
	"fmt"
	"log"
	"os"
	"strings"
	"time"
)

const TRAVAILLEURS int = 10

type paquet struct {
	arrivee string
	depart  string
	arret   int
}

func lecteur(nom string, c chan string) {
	// open the file
	file, err := os.Open("./ressources/" + nom + ".txt")

	//handle errors while opening
	if err != nil {
		log.Fatalf("Error when opening file: %s", err)
	}

	fileScanner := bufio.NewScanner(file)
	first := true
	// read line by line
	for fileScanner.Scan() {
		if first {
			first = false
		} else {
			str := fileScanner.Text()
			c <- str
		}
	}
	// handle first encountered error while reading
	if err := fileScanner.Err(); err != nil {
		log.Fatalf("Error while reading file: %s", err)
	}

	file.Close()
}

func travailleur(s chan string, id int, calc chan paquet, red chan paquet) {
	ligne := <-s
	sep := strings.Split(ligne, "s")
	p := paquet{arrivee: sep[1], depart: sep[2], arret: 0}
	fmt.Println("Travailleur ", id, " recoit ", sep[0], "...")
	calc <- p
	p = <-calc
	red <- p
	fmt.Println("Travailleur ", id, " recoit ensuite: ", sep[1], sep[2], "...")
	travailleur(s, id, calc, red)
}

func calcul(calc chan paquet) {
	for {
		p := <-calc
		go func(p paquet) {
			a, _ := time.Parse("15:02:05", p.arrivee)
			d, _ := time.Parse("15:02:05", p.depart)
			duree := d.Sub(a).Seconds()
			new_p := paquet{arrivee: p.arrivee, depart: p.depart, arret: int(duree)}
			calc <- new_p
		}(p)
	}
}

func main() {
	fmt.Println("Hello world !")
	l := make(chan string)
	calc := make(chan paquet)
	red := make(chan paquet)
	go func() { lecteur("stop_times", l) }()
	for i := 0; i < TRAVAILLEURS; i++ {
		go func(k int) { travailleur(l, k, calc, red) }(i)
	}
	for {
	}

}
