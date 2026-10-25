import os
import csv

# ----------------- User Authentication -----------------

def sign_up():
    print("\n--- Sign Up ---")
    username = input("Enter new username: ")
    password = input("Enter new password: ")
    try:
        salary = float(input("Enter your monthly salary: "))
    except ValueError:
        print("Invalid salary. Please enter a number.\n")
        return
    
    # Check for duplicate username
    if os.path.exists("user.txt"):
        with open("user.txt", "r") as f:
            if any(line.startswith(username + ",") for line in f):
                print("Username already exists. Try a different one.\n")
                return
                
    with open("user.txt", "a") as f:
        f.write(f"{username},{password},{salary}\n")
    print("[Success] Account created successfully!\n")

def login():
    print("\n--- Login ---")
    username = input("Enter username: ")
    password = input("Enter password: ")
    if not os.path.exists("user.txt"):
        print("No users found. Please sign up first.\n")
        return None, None
        
    with open("user.txt", "r") as f:
        users = f.readlines()
        
    for user in users:
        parts = user.strip().split(",")
        if len(parts) == 3:
            u, p, s = parts
            if u == username and p == password:
                print("[Success] Login successful!\n")
                return username, float(s)
                
    print("[Error] Invalid credentials. Try again.\n")
    return None, None

# ----------------- Expense Tracker -----------------

class ExpenseTracker:
    def __init__(self, username):
        self.username = username
        self.filename = "money.csv"
        if not os.path.exists(self.filename):
            with open(self.filename, "w", newline="") as f:
                writer = csv.writer(f)
                writer.writerow(["Username", "Category", "Amount"])
                
    def add_expense(self):
        category = input("Enter category (Food, Travel, etc.): ")
        try:
            amount = float(input("Enter amount: "))
        except ValueError:
            print("Invalid amount. Please enter a number.\n")
            return
            
        with open(self.filename, "a", newline="") as f:
            writer = csv.writer(f)
            writer.writerow([self.username, category, amount])
        print("[Success] Expense added!\n")
        
    def view_expenses(self):
        total = 0
        print("\n--- Your Expenses ---")
        try:
            with open(self.filename, "r") as f:
                reader = csv.reader(f)
                headers = next(reader, None)
                found = False
                for row in reader:
                    if len(row) != 3:
                        continue
                    user, category, amount = row
                    if user == self.username:
                        try:
                            amt = float(amount)
                            print(f"{category} - {amt:.2f}")
                            total += amt
                            found = True
                        except ValueError:
                            continue
                if not found:
                    print("No expenses recorded.")
                else:
                    print(f"Total Expense: {total:.2f}\n")
        except FileNotFoundError:
            print("Expense file not found.\n")
            
    def clear_expenses(self):
        rows = []
        with open(self.filename, "r") as f:
            reader = csv.reader(f)
            rows = [row for row in reader if row[0] != self.username]
        with open(self.filename, "w", newline="") as f:
            writer = csv.writer(f)
            writer.writerow(["Username", "Category", "Amount"])
            writer.writerows(rows)
        print("[Cleared] Your expenses have been cleared!\n")
        
    def get_total_expense(self):
        total = 0
        with open(self.filename, "r") as f:
            reader = csv.reader(f)
            next(reader)
            for row in reader:
                if len(row) == 3 and row[0] == self.username:
                    try:
                        total += float(row[2])
                    except ValueError:
                        continue
        return total

# ----------------- Main Program -----------------

def main_menu(username, salary):
    tracker = ExpenseTracker(username)
    while True:
        print(f"\nLogged in as: {username}")
        print(f"Monthly Salary: {salary}")
        print("1. Add Expense")
        print("2. View Expenses")
        print("3. Clear Expenses")
        print("4. View Savings")
        print("5. Logout")
        choice = input("Enter choice: ")
        
        if choice == "1":
            tracker.add_expense()
        elif choice == "2":
            tracker.view_expenses()
        elif choice == "3":
            tracker.clear_expenses()
        elif choice == "4":
            total = tracker.get_total_expense()
            savings = salary - total
            print(f"\n[Savings] Estimated Savings: {savings:.2f}\n")
        elif choice == "5":
            print("[Locked] Logging out...\n")
            break
        else:
            print("Invalid choice. Try again.\n")

# ----------------- App Loop -----------------

def run_app():
    while True:
        print("{Start} Welcome to Expense Calculator")
        print("1. Login")
        print("2. Sign Up")
        print("3. Exit")
        option = input("Enter option: ")
        
        if option == "1":
            user, salary = login()
            if user:
                main_menu(user, salary)
        elif option == "2":
            sign_up()
        elif option == "3":
            print("[Exit] Exiting... See you again!\n")
            break
        else:
            print("Invalid option. Try again.\n")

# Run the app
run_app()
