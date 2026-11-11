class BufferCondiviso 
{
    private boolean available = false; 
    
    // Variabile per memorizzare effettivamente il dato scambiato
    private int datoCondiviso; 
    
    public synchronized int get() 
    {
        while (available == false) 
        {
            try 
            {
                wait(); // attende un dato dai Produttori
            } 
            catch (InterruptedException e) {}
        }
        
        System.out.println("Consumatore ha letto: " + datoCondiviso);
        
        available = false;
        notifyAll(); // notifica i produttori del consumo
        
        return datoCondiviso;
    }
    
    public synchronized void put(int value) 
    {
        while (available == true) 
        {
            try 
            {
                wait(); // attende il consumo del dato
            } 
            catch (InterruptedException e) {}
        }
        
        datoCondiviso = value;
        System.out.println("Produttore ha inserito: " + datoCondiviso);
        
        available = true;
        notifyAll(); // notifica i consumatori della produzione di un nuovo dato
    }
}