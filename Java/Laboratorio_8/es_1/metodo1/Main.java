import java.io.BufferedReader;
import java.io.IOException;
import java.io.InputStreamReader;

public class Main 
{
    public static void main(String args[]) 
    {
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
            System.exit(-2);
        }
        
        // WorkerThread implementa l'interfaccia Runnable
        WorkerThread wt[] = new WorkerThread[n];
        boolean running[] = new boolean[n];

        for(int i = 0; i < n; i++) 
        {
            wt[i] = new WorkerThread(i);
            Thread t = new Thread(wt[i]);
            t.start();
            running[i] = true;
        }

        BufferedReader br = new BufferedReader(new InputStreamReader(System.in));
        while (checkRunningThreads(running) > 0) 
        {
            System.out.println("Hello, please insert the id of the Thread to terminate: ");
            try 
            {
                int id = Integer.parseInt(br.readLine());
                if (id >= 0 && id < n) 
                {
                    if (running[id] == true) 
                    {
                        wt[id].stop();
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
                System.err.println("Error, not an integer number!");
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


