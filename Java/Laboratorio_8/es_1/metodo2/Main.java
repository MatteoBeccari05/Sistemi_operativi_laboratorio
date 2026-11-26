import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Main 
{
    public static void main(String args[]) {
		
        System.out.println( Thread.currentThread() + " Init...");
        if (args.length != 1 ) 
        {
            System.err.println("Utilizzo: Esercizio1 <numThread>");
            System.exit(-1);
        }
		
        int n = 0;
        try 
        {
            n = Integer.parseInt(args[0]);
        } 
		catch(NumberFormatException e) 
        {
            System.err.println("Utilizzo: Esercizio1 <numThread>");
            System.exit(-1);
        }
        
        // WorketThreadMetodo2 implements Runnable
        WorkerThread wt[] = new WorkerThread[n];
        boolean running[] = new boolean[n];
        Thread t[] = new Thread[n];

        for(int i = 0; i < n; i++) 
        {
            wt[i] = new WorkerThread(i);
            t[i] = new Thread(wt[i]);
            t[i].start();
            running[i] = true;
        }

        while (checkRunningThreads(running) > 0) 
        {
            System.out.println("Hello, please insert the id of the Thread to terminate: ");
            BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
            try 
            {
                int id = Integer.parseInt(br.readLine());
                if (id >= 0 && id < n) 
                {
                    if (running[id] == true) 
                    {
                        wt[id].stop();
                        t[id].interrupt();
                        running[id] = false;
                    }
                }
            }
			catch(IOException e) 
            {
                e.printStackTrace();
            }
			catch(NumberFormatException e) 
            {
                System.err.println("Not an integer number!");
                e.printStackTrace();
            }
        }
    }

    private static int checkRunningThreads(boolean array[]) 
    {
        int runningElements = 0;
        for (boolean e : array) 
        {
            if (e == true) runningElements++;
        }
        return runningElements;
    }
}
