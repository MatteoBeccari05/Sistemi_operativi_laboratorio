import java.util.HashMap;
import java.util.Map;

public class Magazzino 
{
    private Map<String, Integer> oggetti = null;
	
    public Magazzino() 
    {
        oggetti = new HashMap<>();
    }

    // Ogni operazione richiede 500 ms,
    // sleep per implementare tale tempo di esecuzione

    public synchronized boolean verificaOggetto(String object) 
    {
        try 
        {
            Thread.sleep(500);
        } 
        catch(InterruptedException e) {}

        return oggetti.containsKey(object);
    }

    public synchronized void aggiungiOggetto(String object) 
    {
        try 
        {
            Thread.sleep(500);
        } 
        catch(InterruptedException e) {}
        
        if (!verificaOggetto(object))  
        {
            oggetti.put(object, 0);
        }
    }

    public synchronized void increaseQuantity(String object, int qta) 
    {
        if(verificaOggetto(object)) 
        {
            Integer value = oggetti.get(object);
            if (value + qta < 0) 
            {
                System.err.println("Impossibile aggiornare la quantità, dato inserito non corretto");
                return;
            }
            try 
            {
                Thread.sleep(500);
            } 
            catch(InterruptedException e){}
            oggetti.put(object, value + qta);
        } 
        else 
        {
            System.err.println("Oggetto: " + object + " non esiste in magazzino");
        }
    }

    @Override
    public synchronized String toString() 
    {
        String result = "";
        for (Map.Entry<String,Integer> obj : oggetti.entrySet()) 
        {
            result += obj.getKey() + ": " + obj.getValue() + "\n";
        }
        return result;
    }
}