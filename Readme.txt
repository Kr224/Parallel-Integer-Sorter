# Multithreaded Parallel Integer Sorter in C

This C program uses multithreading to sort a list of integers. It splits the list into two sublists, sorts them in parallel using threads, and merges the results into a single sorted list.

## Requirements
- **GCC Compiler**: To compile the program.
- **POSIX Threads (pthread)**: The code uses the `pthread` library for multithreading.
- **Input File**: A file named `IntegerList.txt` in the same directory as the program.

## How to Run

### Step 1: Prepare the Input File
1. Create a file named `IntegerList.txt` in the same directory as your program.
2. Add integers separated by commas (no spaces). Example:
   ```
   7,12,19,3,18,4,2,6,15,8
   ```

### Step 2: Compile the Program
Use `gcc` with the `-lpthread` flag to compile:
```bash
gcc -o main main.c -lpthread
```

### Step 3: Execute the Program
Run the compiled binary:
```bash
./main
```

### Step 4: Check the Output
After execution, the sorted integers will be saved in `SortedIntegerList.txt`. Example output:
```
2,3,4,6,7,8,12,15,18,19
```
---

## Testing

### Test Case 1: Even Number of Integers
**Input (`IntegerList.txt`):**
```
5,1,9,3,7
```
**Expected Output:**
```
1,3,5,7,9
```
**Behavior:**
- Sublist 1 (indices 0–2): `[5,1,9]` → Sorted to `[1,5,9]`
- Sublist 2 (indices 3–4): `[3,7]` → Sorted to `[3,7]`
- Merged Result: `[1,3,5,7,9]`

---

### Test Case 2: Odd Number of Integers
**Input (`IntegerList.txt`):**
```
5,1,9,3,7,8,2
```
**Expected Output:**
```
1,2,3,5,7,8,9
```
**Behavior:**
- Sublist 1 (indices 0–3): `[5,1,9,3]` → Sorted to `[1,3,5,9]`
- Sublist 2 (indices 4–6): `[7,8,2]` → Sorted to `[2,7,8]`
- Merged Result: `[1,2,3,5,7,8,9]`

---

### Test Case 3: Already Sorted Input
**Input (`IntegerList.txt`):**
```
1,2,3,4,5,6,7,8,9,10
```
**Expected Output:**
```
1,2,3,4,5,6,7,8,9,10
```

---

## Notes
1. **Input File Rules**:
   - Integers must be **unique** and in the range `[1, 999]`.
   - No spaces or newlines between integers.
   - Minimum 2 integers, maximum 500 integers.

2. **Thread Behavior**:
   - Two sorting threads work on sublists.
   - One merging thread combines the sorted sublists.
   - For odd-sized lists, the middle element is included in the **first sublist**.

3. **Output File**:
   - The output file (`SortedIntegerList.txt`) is overwritten each run.
