import java.util.concurrent.atomic.AtomicBoolean;

public class WorkerThread implements Runnable 
{
    private final AtomicBoolean running = new AtomicBoolean(false);
	
	private int i;
	public WorkerThread(int i)
    {
		this.i = i;
	}
	
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
                e.printStackTrace();
            }
        }
    }
}