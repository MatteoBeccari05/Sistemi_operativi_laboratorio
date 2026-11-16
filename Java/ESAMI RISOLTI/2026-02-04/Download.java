public class Download {
    private float livello = 0;

    public synchronized float getLivello() {
        return livello;
    }

    public synchronized void setLivello(float livello) {
        this.livello = livello;
    }
}
