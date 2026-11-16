import java.util.concurrent.atomic.AtomicBoolean;

public class SimulaDownload extends Thread {
    private Download download;
    private AtomicBoolean isRunning = new AtomicBoolean(true);

    public SimulaDownload(Download download) {
        this.download = download;
    }

    public void run() {
        isRunning.set(true);

        while (isRunning.get()) {
            // genero un valore casuale tra 0 e 0.25 (estremi compresi)
            float incremento = (float) (Math.random() * 0.25);

            // aggiungo l'incremento al livello corrente (max 1)
            float livelloCorrente = download.getLivello();
            float nuovoLivello = livelloCorrente + incremento;
            if (nuovoLivello > 1f) {
                nuovoLivello = 1f;
            }
            download.setLivello(nuovoLivello);

            // attendo 1 secondo
            try {
                Thread.sleep(1000);
            } catch (InterruptedException e) {
                e.printStackTrace();
            }
        }
    }

    public void termina() {
        isRunning.set(false);
    }
}
