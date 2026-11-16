import java.io.PipedInputStream;
import java.nio.charset.Charset;
import java.util.Arrays;

public class MonitoraLogin extends Thread {

    private PipedInputStream pis;

    public MonitoraLogin(PipedInputStream pis) {
        this.pis = pis;
    }

    @Override
    public void run() {
        byte buffer[] = new byte[1024];
        int nread = 0;

        try {
            while ((nread = pis.read(buffer)) > 0) {
                byte message_buffer[] = Arrays.copyOfRange(buffer, 0, nread);
                String message = new String(message_buffer, Charset.forName("UTF-8"));
                int numeroTentativi = Integer.parseInt(message);

                System.out.println("Tentativi di login arrivati: " + numeroTentativi);

                if (numeroTentativi > 20) {
                    System.out.println("ATTENZIONE: Possibile attacco informatico! Tentativi: "
                                    + numeroTentativi);
                }
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
        System.out.println("MoniotoraLogin terminato");
    }
}
