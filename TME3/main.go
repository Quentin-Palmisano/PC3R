package main
import (
    "bufio"
    "fmt"
    "log"
    "os"
)

const TRAVAILLEURS int = 10

type horaire struct {
	heure int
	minute int
	seconde int
}

func newHoraire(heure int, minute int, seconde int) *horaire {

    h := horaire{}
    h.heure = heure
	h.minute = minute
	h.seconde = seconde
    return &h
}

type paquet struct {
    arrivee horaire
    depart horaire
	arret horaire
}

func newPaquet(arrivee horaire, depart horaire) *paquet {

    p := paquet{}
    p.arrivee = arrivee
	p.depart = depart
	p.arret = newHoraire(0, 0, 0)
    return &p
}

func lecteur (nom string){
	// open the file
    file, err := os.Open("./ressources"+nom+".txt")

    //handle errors while opening
    if err != nil {
        log.Fatalf("Error when opening file: %s", err)
    }


    fileScanner := bufio.NewScanner(file)
	first := true
    // read line by line
    for fileScanner.Scan() {
		if first{
			first = false
		}else{
			fmt.Println(fileScanner.Text())
		}
    }
    // handle first encountered error while reading
    if err := fileScanner.Err(); err != nil {
        log.Fatalf("Error while reading file: %s", err)
    }

    file.Close()
}
//trip_id,arrival_time,departure_time,stop_id,stop_sequence,stop_headsign,pickup_type,drop_off_type,shape_dist_traveled
//OCESN998F535973:2022-01-28T20:47:56Z,18:40:00,18:40:00,StopPoint:OCETGV INOUI-87471003,10,,1,0,

func getArrivee(str string){
	h := 0
	m := 0
	s := 0
	for i := 0; i < len(str); i++ {
		if(str[i]==','){
			for j := i+1; j < i+9; j++ {
				//intVar, err := strconv.Atoi(strVar)
			}
		}
		
	 }
}

func travailleur(ligne string){

}

func main(){
	fmt.Println("Hello world !")
	go func () {lecteur("./ressources/stop_times.txt")}()
	for{}
}