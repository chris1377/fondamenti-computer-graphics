# Fondamenti di Grafica - Portfolio Laboratori

Questo repository raccoglie i progetti e le esercitazioni sviluppati per il corso di Fondamenti di Grafica. Il percorso spazia dallo sviluppo di applicazioni grafiche interattive 2D e 3D (tramite shader personalizzati) fino alla modellazione e al rendering avanzato utilizzando Blender.

Di seguito è riportata la descrizione dettagliata di ciascun laboratorio.

---

## LAB 1: Curve 2D (Bézier e Catmull-Rom)
Applicazione interattiva per il disegno di curve libere basate su punti di controllo inseriti dall'utente.
- **Modalità Approssimazione (Bézier):** La curva si avvicina ai punti inseriti, calcolando i segmenti tramite l'algoritmo di De Casteljau.
- **Modalità Interpolazione (Catmull-Rom):** Tramite un'apposita funzione, la curva passa esattamente per i punti di controllo, calcolando la pendenza e i punti intermedi necessari.
- **Interattività:** È possibile spostare i punti di controllo trascinandoli con il mouse. Lo switch tra le due modalità di disegno avviene tramite la pressione del tasto `c`.

---

## LAB 2: Videogioco 2D "Flappy Duck"
Un minigioco interattivo ispirato a Flappy Bird, sviluppato a partire da un progetto di base e arricchito con meccaniche personalizzate e shader.
- **Gameplay e Fisica:** Controllo del personaggio (un'anatra gialla) tramite il tasto `W` per volare, con una simulazione di caduta gravitazionale quando il tasto viene rilasciato. Gli ostacoli (colonne verdi) hanno posizione randomica per lo spazio di passaggio.
- **Sistema di Collisioni:** Rilevamento preciso basato sulle coordinate X e Y del personaggio rispetto ai margini degli ostacoli. In caso di collisione, il gioco termina e viene stampato il punteggio.
- **Sistema Particellare:** Una scia di particelle dinamiche segue il personaggio durante il volo, sfumando dall'azzurro al verde e modificando il valore alpha.
- **Custom Shaders:** Implementazione di gradienti per dare profondità agli ostacoli e uno shader animato per le nuvole in background.

---

## LAB 3: Scena 3D Interattiva e Shading
Composizione di una scena 3D (Tè del pomeriggio) che implementa diverse tecniche di illuminazione e interazione.
- **Mesh e Scansioni 3D:** Oltre a modelli standard (tavolo, tazza, teiera), la scena include una mesh scansionata dal vero ("Victory Royale"), elaborata e ottimizzata.
- **Illuminazione (Phong Shading):** Implementazione di luce ambientale, diffusa e speculare all'interno del Vertex e Fragment shader.
- **Toon Shading:** Uno shader personalizzato che quantizza la componente diffusa della luce per ottenere un effetto grafico stilizzato (a bande di colore).
- **Animazioni Vertex-based (Wave Motion):** Deformazione dinamica della geometria nel tempo calcolata direttamente nel vertex shader (tramite funzioni seno).
- **Interazione e Navigazione:** 
  - **Picking:** Selezione degli oggetti nella scena tramite raggio (Raycasting) e calcolo delle intersezioni con sfere invisibili ancorate ai modelli.
  - **Trasformazioni:** Traslazione, rotazione e scaling degli oggetti selezionati.
  - **Camera su Curva:** Animazione automatica della telecamera che segue un percorso ciclico definito da una curva di Bézier (attivabile con `B`, disattivabile con `N`).

---

## LAB 4: Modellazione 3D Procedurale e Fisica (Blender)
Creazione di una scena "Mercato" con diverse tecniche di modellazione avanzata.
- **Modellazione Poligonale:** Banchetto, sacchi, gazebo e candele creati tramite estrusioni, Loop Cut, Subdivision Surface e strumenti come lo *Spin* (per i piatti).
- **Modificatori Avanzati:** Utilizzo del modificatore *Screw* e *Curve* per realizzare una corda intrecciata. Il terreno sabbioso è stato ottenuto combinando i modificatori *Displace* (con texture "Clouds") e *Smooth*.
- **Scultura Digitale:** Utilizzo della Sculpt Mode con *Dyntopo* per scolpire mucchi di spezie dall'aspetto organico.
- **Nodi e Fisica:**
  - **Rigid Body Physics:** Utilizzata per far cadere e posizionare realisticamente le arance (sfere duplicate) nel banchetto.
  - **Geometry Nodes:** Replicazione procedurale dei piatti su una griglia tramite nodi `Grid` e `Instance on Points`.

---

## LAB 5: Materiali Procedurali e Rendering (Blender)
Allestimento e rendering di una scena natalizia focalizzata sulla creazione di materiali complessi basati su nodi.
- **Materiali Procedurali:**
  - **Cera:** Shader *Principled BSDF* con Subsurface Scattering per simulare la traslucenza, combinato con una *Noise Texture* per i rilievi superficiali (bump).
  - **Fiamma:** Materiale emissivo gestito da *Gradient Texture*, *Layer Weight* e *Color Ramp* per l'effetto di trasparenza ai bordi e sfumatura cromatica.
  - **Vetro (Snow Globe):** Materiale trasparente con leggere imperfezioni controllate da nodi di rumore (Noise Texture e Bump).
  - **Materiali Metallici e Opachi:** Palline di natale con diversi gradi di metallicità e *Roughness*.
- **Motori di Rendering:** Confronto tecnico e qualitativo tra Eevee (rasterizzazione veloce) e Cycles (ray-tracing accurato, riflessioni e rifrazioni fotorealistiche).

---

## LAB 6: Shader Mapping ed Effetti Avanzati
Esplorazione approfondita della gestione di texture e ambienti virtuali.
- **Skybox (CubeMap):** Creazione di un ambiente sferico campionando 6 texture senza subire la traslazione della camera.
- **Texture Mapping 2D e Shading:** Mappatura di texture UV applicate a primitive 3D (come un toroide) integrate con un modello di illuminazione Phong per maggiore realismo.
- **Riflessione (Reflection):** Shader per riflettere l'ambiente della Skybox sulle superfici degli oggetti calcolando il vettore di riflessione.
- **Rifrazione (Refraction):** Simulazione della distorsione della luce attraverso un materiale trasparente modificando l'indice di rifrazione nel campionamento.
- **Texture Procedurali:** Generazione matematica del colore direttamente nello shader (es. bandiera del Giappone generata calcolando la distanza euclidea dal centro per definire il cerchio rosso).
