import java.util.concurrent.atomic.AtomicBoolean;

public class WorkerThread implements Runnable 
{
    private final AtomicBoolean running = new AtomicBoolean(false);
	
	private int i;
	public WorkerThread(int i)
    {
		this.i = i;
	}
	
    // stop the thread
    public void stop() 
    {
        running.set(false);
    }

    @Override
    public void run() 
    {
        running.set(true);
        while(running.get()) 
        {
            try 
            {
				System.out.println("Thread: " + i);
                Thread.sleep(1000);
            }
			catch(InterruptedException e)
            {
                System.out.println(Thread.currentThread() + " interrupted, terminating...");
            }
        }
    }
}