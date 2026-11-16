import java.io.IOException;
import java.io.PipedOutputStream;
import java.nio.charset.Charset;
import java.util.concurrent.atomic.AtomicBoolean;

public class SimulaLogin extends Thread {
	
    private PipedOutputStream pos;
    private AtomicBoolean isRunning = new AtomicBoolean(true);

    public SimulaLogin(PipedOutputStream pos) {
        this.pos = pos;
    }

    @Override
    public void run() {
		
        while (isRunning.get()) {
            try {
                int numeroTentativi = (int) (Math.random() * 51);

                // serializzazione manuale
                String msg  = "" + numeroTentativi;
                byte[] data = msg.getBytes(Charset.forName("UTF-8"));
                pos.write(data, 0, data.length);
                pos.flush();
                System.out.println("Valore rilevato: " + numeroTentativi);

                Thread.sleep(10*1000); // 10 secondi
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
		
        try {
            pos.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
		
        System.out.println("SimulaLogin terminato");
    }

    public void termina() {
        isRunning.set(false);
    }
}
