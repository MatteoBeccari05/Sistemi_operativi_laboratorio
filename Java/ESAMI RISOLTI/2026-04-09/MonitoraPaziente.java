import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;
import java.io.PipedInputStream;
import java.util.concurrent.atomic.AtomicBoolean;

public class MonitoraPaziente implements Runnable {
    private PipedInputStream pis = null;
    private BufferedReader br = null;
    private final AtomicBoolean isRunning = new AtomicBoolean(false);

    public MonitoraPaziente(PipedInputStream pis) {
        this.pis = pis;
        // Creazione dello stream bufferizzato (Reader)
        this.br = new BufferedReader(new InputStreamReader(this.pis));
    }

    @Override
    public void run() {
        isRunning.set(true);
        try {
            String line;
            // Leggiamo riga per riga finché disponibile
            while (isRunning.get() && (line = br.readLine()) != null) {
                int bpm = Integer.parseInt(line);

                // Stampa del valore attuale
                System.out.println("Frequenza cardiaca: " + bpm + " bpm");

                // Messaggio di avviso se bpm > 120
                if (bpm > 120) {
                    System.out.println("warning: rischio tachicardia (" + bpm + " bpm)");
                }
            }
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
