import java.io.*;

public class FromInput extends Thread
{
    private PipedOutputStream pos;

    public FromInput(PipedOutputStream pos)
    {
        this.pos = pos;
    }

    public void run()
    {
        try
        {
            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            String line;
            while((line = br.readLine()) != null && !line.equals("exit"))
            {
                byte[] buffer = line.getBytes();
                pos.write(buffer);
                pos.flush();
            }
            pos.close();
        }
        catch (IOException ex)
        {
            ex.printStackTrace();
        }
    }
}