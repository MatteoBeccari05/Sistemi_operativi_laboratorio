
import java.io.IOException;
import java.io.PipedInputStream;

public class ToOutput extends Thread
{
    private PipedInputStream pis;

    public ToOutput(PipedInputStream pis)
    {
        this.pis = pis;
    }
    
    public void run()
    {
        try
        {
            byte[] buffer = new byte[1024];
            int byteLetti;
            while((byteLetti = pis.read(buffer)) != -1)
            {
                String messaggio = new String(buffer, 0, byteLetti);
                System.out.println("[ToOutput - Byte]: " + messaggio);
            }
            pis.close();
        }
        catch(IOException ex)
        {
            ex.printStackTrace();
        }
    }
}
