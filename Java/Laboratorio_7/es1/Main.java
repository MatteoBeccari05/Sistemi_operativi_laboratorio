import java.util.Scanner;

public class Main
{
    public static void main(String[] args) 
    {
        Scanner s = new Scanner(System.in);
        System.out.print("Inserire il numero di thread da creare: ");
        int n = s.nextInt();
        s.close();

        Accumulatore accumulatore = new Accumulatore(0.0);

        //creo e lancio n thread
        CounterThread[] threads = new CounterThread[n];
        for(int i = 0; i < n; i++)
        {
            threads[i] = new CounterThread(accumulatore);
            threads[i].start();
        }

        for(int i = 0; i < n; i++)
        {
            try
            {
                threads[i].join();
            }
            catch (InterruptedException ex)
            {
                ex.printStackTrace();
            }
        }

        System.out.println("Valore finale accumulatore: " + accumulatore.getValue());
    }
}