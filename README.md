# Expense Tracker: A Python to C Migration Journey 🚀

A terminal-based expense tracking system with user authentication, salary tracking, and expense logging. 

This project represents my growth transition as a first-year Computer Science & Engineering (Core) student—taking logic I built using a high-level scripting language (Python) and porting it into a low-level systems language (C) to deeply understand memory management.

## 🛠️ Features
- **User Authentication:** Multi-user registration and login validations.
- **Persistent Storage:** Saves user info and expenditure logs securely via file handling.
- **Financial Metrics:** Computes monthly totals and estimates remaining savings against salary.

## 🔄 The Python vs. C Learning Curve
Porting this application forced me to learn how core computer science concepts differ under the hood:
1. **Data Structures:** Transitioned from Python's dynamic classes and lists to C's procedural functions and structured arrays (`struct`).
2. **Memory Management:** Handled strings manually via fixed char arrays (`char username[50]`) and ensured active file pointers were closed safely with `fclose()`.
3. **Data Passing:** Practiced using memory addresses and pointers (`*` and `&`) to update variables across functions in C.

## 🚀 How to Run

### Python Version
```bash
python python-version/expense_tracker.py
```

### C Version
```bash
gcc c-version/expense_tracker.c -o expense_tracker
./expense_tracker
```
