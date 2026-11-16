package esame.es2;

import java.io.IOException;
import java.io.ObjectInputStream;
import java.io.PipedInputStream;
import java.util.concurrent.atomic.AtomicBoolean;

public class Quality extends Thread{

    private PipedInputStream pis = null;
    private Overall overall = null;
    private AtomicBoolean isRunning = new AtomicBoolean(false);

    public Quality(PipedInputStream pis, Overall overall){
        this.pis = pis;
        this.overall = overall;
    }
    
    public void run() {
        isRunning.set(true);

        // leggo i dati dal thread Machine
        ObjectInputStream ois = null;
        try {
            ois = new ObjectInputStream(pis);
        } catch(IOException e) {
            e.printStackTrace();
            System.exit(-1);
        }

        while(isRunning.get()) {
            
            try {
                Message m = (Message)ois.readObject();
                
                float findicator = m.getFIndicator();

                if (findicator > 0) {
                    System.out.println("OK");
                    overall.incrementaCorretti();
                } else {
                    System.out.println("Errore, per pezzo numero " + m.getPezziProdotti());
                    overall.incrementaDifetti();
                }
            } catch(IOException | ClassNotFoundException e) {
                e.printStackTrace();
            }

        }
    }

    public void termina() {
        isRunning.set(false);
    }
}
