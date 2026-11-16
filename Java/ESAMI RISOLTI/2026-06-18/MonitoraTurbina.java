import java.io.ObjectInputStream;
import java.io.PipedInputStream;

public class MonitoraTurbina extends Thread {
    private PipedInputStream pis;

    public MonitoraTurbina(PipedInputStream pis) {
        this.pis = pis;
    }

    @Override
    public void run() {
        ObjectInputStream ois = null;
        try {
            ois = new ObjectInputStream(pis);
        } catch (Exception e) {
            e.printStackTrace();
            return;
        }

        while (true) {
            try {
                VelocitaAttuale velocita = (VelocitaAttuale) ois.readObject();
                if (velocita != null) {
                    System.out.println("Velocita vento: " + velocita.getVelocitaVento() + " km/h" +
                                       " - Timestamp: " + velocita.getTimestamp());
                    if (velocita.getVelocitaVento() > 90) {
                        System.out.println("ATTENZIONE: Rischio di danneggiamento! Velocita vento: "
                                           + velocita.getVelocitaVento() + " km/h");
                    }
                }
            } catch (Exception e) {
                e.printStackTrace();
                break;
            }
        }
    }
}
