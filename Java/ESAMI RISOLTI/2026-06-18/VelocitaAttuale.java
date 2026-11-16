import java.io.Serializable;

public class VelocitaAttuale implements Serializable {
    private int velocitaVento;
    private long timestamp;

    public VelocitaAttuale(int velocitaVento, long timestamp) {
        this.velocitaVento = velocitaVento;
        this.timestamp = timestamp;
    }

    public int getVelocitaVento() {
        return velocitaVento;
    }

    public void setVelocitaVento(int velocitaVento) {
        this.velocitaVento = velocitaVento;
    }

    public long getTimestamp() {
        return timestamp;
    }

    public void setTimestamp(long timestamp) {
        this.timestamp = timestamp;
    }
}
