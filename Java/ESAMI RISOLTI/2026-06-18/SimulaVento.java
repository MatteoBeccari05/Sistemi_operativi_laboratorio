import java.io.ObjectOutputStream;
import java.io.PipedOutputStream;
import java.util.concurrent.atomic.AtomicBoolean;

public class SimulaVento extends Thread {
    private PipedOutputStream pos;
    private AtomicBoolean isRunning = new AtomicBoolean(true);

    public SimulaVento(PipedOutputStream pos) {
        this.pos = pos;
    }

    @Override
    public void run() {
        ObjectOutputStream oos = null;
        try {
            oos = new ObjectOutputStream(pos);
        } catch (Exception e) {
            e.printStackTrace();
            return;
        }

        while (isRunning.get()) {
            try {
                int velocitaVento = (int) (Math.random() * 121); // Genera numeri tra 0 e 120
                VelocitaAttuale velocita = new VelocitaAttuale(velocitaVento, System.currentTimeMillis());
                oos.writeObject(velocita);
                oos.flush();

                Thread.sleep(4000); // Attende 4 secondi
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public void termina() {
        isRunning.set(false);
        Thread.currentThread().interrupt();
    }
}
