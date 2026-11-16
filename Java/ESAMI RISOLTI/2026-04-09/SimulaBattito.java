import java.io.BufferedWriter;
import java.io.IOException;
import java.io.OutputStreamWriter;
import java.io.PipedOutputStream;
import java.util.concurrent.atomic.AtomicBoolean;

public class SimulaBattito implements Runnable {
    private PipedOutputStream pos = null;
    private BufferedWriter bw = null;
    private final AtomicBoolean isRunning = new AtomicBoolean(false);

    public SimulaBattito(PipedOutputStream pos) {
        this.pos = pos;
        // Creazione dello stream bufferizzato (Writer)
        this.bw = new BufferedWriter(new OutputStreamWriter(this.pos));
    }

    @Override
    public void run() {
        isRunning.set(true);
        while (isRunning.get()) {
            try {
                Thread.sleep(2000);
                // Generazione di un valore tra 40 e 200
                int bpm = 40 + (int) (Math.random() * 161);

                // Scriviamo la frequenza come riga di testo
                bw.write(Integer.toString(bpm));
                bw.newLine();      // Termine riga
                bw.flush();

                // Stampa del valore rilevato (solo a scopo di debug)
                System.out.println("valore rilevato: " + bpm);
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (IOException e) {
                // In caso di chiusura del PipedOutputStream può sollevare IOException
                e.printStackTrace();
                break;
            }
        }
    }

    public void termina() {
        isRunning.set(false);
        try {
            // Chiude lo stream bufferizzato
            bw.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
