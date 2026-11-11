public class Main 
{
    public static void main(String[] args) 
    {
        // 1. Creazione dell'oggetto condiviso
        BufferCondiviso buffer = new BufferCondiviso();
        
        // 2. Creazione dei thread passando loro il riferimento alla risorsa condivisa
        Produttore p1 = new Produttore(buffer);
        Consumatore c1 = new Consumatore(buffer);
        
        // 3. Avvio dell'esecuzione concorrente
        p1.start();
        c1.start();
    }
}