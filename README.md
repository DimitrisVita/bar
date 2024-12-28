# OS PROJECT 2: NEMEA BAR

**Όνομα:** Βιταντζάκης Δημήτριος

**Αριθμός Μητρώου:** 1115202200019

## Περιγραφή

Αυτό το έργο προσομοιώνει τη λειτουργία ενός μπαρ με πολλούς επισκέπτες και έναν ρεσεψιονίστ. Οι επισκέπτες κάθονται σε τραπέζια, κάνουν παραγγελίες και ξεκουράζονται για κάποιο χρονικό διάστημα πριν φύγουν. Ο ρεσεψιονίστ επεξεργάζεται τις παραγγελίες των επισκεπτών.

## Οδηγίες Εκτέλεσης

1. **Μεταγλώττιση του έργου:**
    ```sh
    make all
    ```

2. **Εκκίνηση του προσομοίωσης:**
    ```sh
    ./smem_init
    ```

3. **Παρακολούθηση της κατάστασης του μπαρ:**
    ```sh
    ./monitor -s <shmid>
    ```

## Σημαφόροι

Οι σημαφόροι χρησιμοποιούνται για τον συγχρονισμό των διαδικασιών και την αποφυγή racing conditions. Στο συγκεκριμένο έργο, χρησιμοποιούνται οι παρακάτω σημαφόροι:

- `mutex`: Χρησιμοποιείται για την προστασία της shared memory από ταυτόχρονες προσβάσεις. Όταν μια διαδικασία θέλει να διαβάσει ή να γράψει στην shared memory, πρέπει πρώτα να αποκτήσει τον σημαφόρο `mutex`.

- `sit`: Χρησιμοποιείται για να ελέγχει τη διαθεσιμότητα των καρεκλών. Όταν ένας επισκέπτης θέλει να καθίσει, πρέπει να αποκτήσει τον σημαφόρο `sit`, ο οποίος μειώνεται κατά 1. Όταν ο επισκέπτης φεύγει, ο σημαφόρος αυξάνεται κατά 1. Η αρχική του τιμή είναι ίση με: `NUM_TABLES * CHAIRS_PER_TABLE`.

- `order`: Χρησιμοποιείται για να συγχρονίζει την παραγγελία του επισκέπτη με τον ρεσεψιονίστ. Ο επισκέπτης περιμένει να αποκτήσει τον σημαφόρο `order` για να λάβει την παραγγελία του.

- `wakeup`: Χρησιμοποιείται για να ξυπνάει τον ρεσεψιονίστ όταν φτάνει ένας νέος επισκέπτης. Ο επισκέπτης αυξάνει τον σημαφόρο `wakeup` για να ειδοποιήσει τον ρεσεψιονίστ.

- `log`: Χρησιμοποιείται για τον συγχρονισμό του logging στο αρχείο καταγραφής. Όταν μια διαδικασία θέλει να καταγράψει ένα μήνυμα, πρέπει πρώτα να αποκτήσει τον σημαφόρο `log`.

## Στατιστικά

- `Average visit duration`: Ο μέσος χρόνος παραμονής των επισκεπτών στο μπαρ. Υπολογίζεται από τη στιγμή που ο επισκέπτης φτάνει στο μπαρ μέχρι τη στιγμή που φεύγει.
- `Average wait duration`: Ο μέσος χρόνος αναμονής των επισκεπτών για να βρουν καρέκλα. Υπολογίζεται από τη στιγμή που ο επισκέπτης φτάνει στο μπαρ μέχρι τη στιγμή που βρίσκει καρέκλα.

## Αρχεία

- `visitor.c`: Κώδικας για τους επισκέπτες που κάθονται, κάνουν παραγγελίες και φεύγουν.
- `receptionist.c`: Κώδικας για τον ρεσεψιονίστ που επεξεργάζεται τις παραγγελίες των επισκεπτών.
- `smem_utils.c`: Βοηθητικές συναρτήσεις για τη διαχείριση της shared memory και των σημάτων.
- `smem_init.c`: Αρχικοποίηση της shared memory και εκκίνηση των processes.
- `monitor.c`: Εμφάνιση στατιστικών και κατάστασης των τραπεζιών.
- `common_types.h`: Κοινές δομές δεδομένων και σταθερές.

## Συναρτήσεις και πολυπλοκότητά

### visitor.c

- `find_chair(SharedMemory* shm)`: Αναζήτηση κενής καρέκλας.
  - **Complexity**: O(n), με n ο αριθμός των καρεκλών.
  - **Logic**: Αναζητά την πρώτη κενή καρέκλα και την καταλαμβάνει.

- `leave_chair(SharedMemory* shm)`: Αποχώρηση από την καρέκλα.
  - **Complexity**: O(n), με n ο αριθμός των καρεκλών.
  - **Logic**: Αποδεσμεύει την καρέκλα που καθόταν ο visitor. Στη περίπτωση που όλες οι καρέκλες σε ένα τραπέζι έχουν αποδεσμευτεί, οι θέσεις του τραπεζιού γίνονται ξανά διαθέσιμες.

### smem_utils.c

- `create_shmem()`: Δημιουργία τμήματος shared memory.
  - **Complexity**: O(1)
  - **Logic**: Δημιουργεί ένα νέο τμήμα shared memory και επιστρέφει το id του.

- `attach_shmem(int shmid)`: Προσάρτηση τμήματος shared memory στη διαδικασία.
  - **Complexity**: O(1)
  - **Logic**: Προσαρτά το τμήμα shared memory στη διαδικασία και επιστρέφει έναν δείκτη σε αυτό.

- `detach_shmem(SharedMemory* shm)`: Αποπροσάρτηση τμήματος shared memory από το process.
  - **Complexity**: O(1)
  - **Logic**: Αποπροσαρτά το τμήμα shared memory από το process.

- `destroy_shmem(int shmid)`: Καταστροφή τμήματος shared memory.
  - **Complexity**: O(1)
  - **Logic**: Καταστρέφει το τμήμα shared memory και τους σχετικούς σημαφόρους.

- `init_semaphores(SharedMemory* shm)`: Αρχικοποίηση σημάτων.
  - **Complexity**: O(1)
  - **Logic**: Αρχικοποιεί τους σημαφόρους που χρησιμοποιούνται για τον συγχρονισμό.

- `log_message(SharedMemory* shm, const char* format, ...)`: Καταγραφή μηνύματος στο αρχείο καταγραφής.
  - **Complexity**: O(1)
  - **Logic**: Καταγράφει ένα μήνυμα στο αρχείο καταγραφής με χρήση σημαφόρου για τον συγχρονισμό της εγγραφής.

### smem_init.c

- `init_statistics(SharedMemory* shm)`: Αρχικοποίηση στατιστικών.
  - **Complexity**: O(1)
  - **Logic**: Αρχικοποιεί τις μεταβλητές στατιστικών.

- `init_tables(SharedMemory* shm)`: Αρχικοποίηση τραπεζιών.
  - **Complexity**: O(n)
  - **Logic**: Αρχικοποιεί τις καρέκλες σε κάθε τραπέζι.

### monitor.c

- `print_statistics(SharedMemory* shm)`: Εκτύπωση στατιστικών.
  - **Complexity**: O(1)
  - **Logic**: Εκτυπώνει τις τρέχουσες στατιστικές.

- `print_tables(SharedMemory* shm)`: Εκτύπωση κατάστασης τραπεζιών.
  - **Complexity**: O(n)
  - **Logic**: Εκτυπώνει την κατάσταση κάθε τραπεζιού και καρέκλας.
