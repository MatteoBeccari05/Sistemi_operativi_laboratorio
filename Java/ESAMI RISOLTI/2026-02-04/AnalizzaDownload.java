import java.util.concurrent.atomic.AtomicBoolean;

public class AnalizzaDownload extends Thread {
    private Download download;
    private AtomicBoolean isRunning = new AtomicBoolean(true);

    public AnalizzaDownload(Download download) {
        this.download = download;
    }

    public void run() {
        isRunning.set(true);

        while (isRunning.get()) {
            // leggo il livello di avanzamento
            float livello = download.getLivello();

            // stampo il livello di avanzamento (come percentuale)
            System.out.println("Avanzamento download: " + (livello * 100) + "%");

            // stampo warning se il livello supera 0.8
            if (livello > 0.8f) {
                System.out.println("WARNING: download quasi completato");
            }

            // attendo 2 secondi
            try {
                Thread.sleep(2000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void termina() {
        isRunning.set(false);
    }
}
