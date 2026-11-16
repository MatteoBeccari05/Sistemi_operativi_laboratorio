package esame.es2;

public class Overall {

    private int corretti = 0;
    private int difetti = 0;

    public synchronized void incrementaCorretti() {
        this.corretti++;
    }

    public synchronized void incrementaDifetti() {
        this.difetti++;
    }

    public synchronized int getDifetti() {
        return this.difetti;
    }

    public synchronized int getCorretti() {
        return this.corretti;
    }
    
}
