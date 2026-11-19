import java.io.*;

public class Main
{
    public static void main(String[] args) throws IOException
    {
        PipedOutputStream pos = new PipedOutputStream();
        PipedInputStream pis = new PipedInputStream(pos);

        FromInput invio = new FromInput(pos);
        ToOutput ricevente = new ToOutput(pis);

        invio.start();
        ricevente.start();
    }
}
