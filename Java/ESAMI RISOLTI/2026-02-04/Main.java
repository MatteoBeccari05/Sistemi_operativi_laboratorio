public class Main {
    public static void main(String[] args) {
        Download download = new Download();

        float livello = 0f;
        while (true) {
            System.out.println("Inserire avanzamento iniziale (tra 0 e 1): ");
            try {
                livello = Float.parseFloat(System.console().readLine());
                // controllo che il valore rientri tra 0 e 1
                if (livello >= 0f && livello <= 1f) {
                    break;
                }
            } catch (NumberFormatException e) {
                System.out.println("Inserire un numero float compreso tra 0 e 1");
            }
        }
        // imposto il livello iniziale nella struttura condivisa
        download.setLivello(livello);

        // creazione dei thread
        SimulaDownload simulaDownload = new SimulaDownload(download);
        AnalizzaDownload analizzaDownload = new AnalizzaDownload(download);

        // avvio dei thread
        simulaDownload.start();
        analizzaDownload.start();

        // attendo 10 secondi
        try {
            Thread.sleep(10000);
        } catch (InterruptedException e) {
            e.printStackTrace();
        }

        // interrompo i thread
        simulaDownload.termina();
        analizzaDownload.termina();

        // attendo la loro terminazione
        try {
            simulaDownload.join();
            analizzaDownload.join();
        } catch (InterruptedException e) {
            e.printStackTrace();
        }
    }
}
