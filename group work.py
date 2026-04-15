## Baker's Cove - Employee Payroll System
## Course: Programming 2 - CMP1025
##
##
## --- Group Members ---
##
## Dejahnae Fairclough - 2406846
## Javia Clarke - 2209358
## Ougaba Gray - 2405837
## Daeshaun McIntyre - 2403178
##
## Tutor: Christopher Panther
##
## Payroll Frequency: Monthly
##
## This program oversees the employee payroll records side of the system.
## It loads the empolyees into the system, allows viewing via filters of employees who are Active/Terminated/On-Leave.
## All the information is saved to payroll.csv 

import datetime
import random # random number generation seeding
import os

employees = [] 

def load_employees():
    global employees
    employees.clear()

     
    if not os.path.exists("employee.csv"):
        print("Error: 'employee.csv' is missing.")
        return

    try: 
        with open("employee.csv", "r") as employee_file:
            lines = employee_file.readlines()
            if not lines:
                print("No employees found: 'employee.csv' is empty.")
                return
            
            for line in lines:
                data = line.strip().split(",")
                if len(data) >= 5:
                    employees.append({
                        "id": data[0],
                        "first_name": data[1],
                        "last_name": data[2],
                        "role": data[3],
                        "status": data[4]
                    })
            print(f"Successfully loaded {len(employees)} employees into memory.")
    except Exception as e:
        print(f"An error occurred while loading: {e}")

def view_employees():
    if not employees:
        print("No employees loaded. Return to menu.")
        return

    status_filter = input("Filter by status (Active/On Leave/Terminated): ").strip().lower()
    print(f"\n--- {status_filter.upper()} EMPLOYEE LIST ---")
    
    for emp in employees:
        if emp["status"].lower() == status_filter:
            print(f"ID: {emp['id']} | Name: {emp['first_name']} {emp['last_name']} | Role: {emp['role']}")
            
            #  Displaying last payslip info from payroll.csv 
            found_payslip = False
            if os.path.exists("payroll.csv"):
                with open("payroll.csv", "r") as f:
                    for line in f:
                        p_data = line.strip().split(",")
                        if p_data[1] == emp['id']:
                            print(f"  [Last Payslip] Date: {p_data[0]} | Net: ${p_data[7]}")
                            found_payslip = True
            
            if not found_payslip:
                print("  [Last Payslip] None")

def calculate_payroll():
    if not employees:
        print("No employees loaded. Payroll blocked.")
        return

    #  transactions are being saved in payroll.csv using consistent format 
    with open("payroll.csv", "a") as payroll_file:
        for emp in employees:
#  Only Active status allowed. Terminated/On Leave blocked 
            if emp["status"].lower() != "active":
                print(f"Skipping {emp['id']}: Status '{emp['status']}' is ineligible for payroll.")
                continue

            print(f"\nProcessing {emp['first_name']} {emp['last_name']} ({emp['id']})")
            try:
                hours = float(input("  Enter hours worked: "))
                rate = float(input("  Enter hourly rate: "))
                
                # Project Tax Model
                gross = hours * rate
                nis = gross * 0.025 # 2.5% of gross
                edu_tax = (gross - nis) * 0.0225 # 2.25% applied after NIS
                net = gross - nis - edu_tax
                
                date = str(datetime.date.today())
                # Format: Date, ID, Hours, Rate, Gross, NIS, EduTax, Net
                payroll_file.write(f"{date},{emp['id']},{hours},{rate},{gross:.2f},{nis:.2f},{edu_tax:.2f},{net:.2f}\n")
                print(f"  Payroll Processed. Net Pay: ${net:.2f}")
                
            except ValueError:
                print("  Invalid input. Skipping employee.")

def payroll_reports():
    
    if not os.path.exists("payroll.csv"):
        print("No payroll history found.")
        return

    total_net = 0.0
    total_deductions = 0.0
    top_earners = []

    with open("payroll.csv", "r") as f:
        for line in f:
            data = line.strip().split(",")
            total_net += float(data[7])
            total_deductions += (float(data[5]) + float(data[6]))
            top_earners.append((data[1], float(data[7])))

    print("\n--- FINANCIAL SUMMARY REPORT ---")
    print(f"Total Net Pay (All Time): ${total_net:.2f}")
    print(f"Total Deductions Collected: ${total_deductions:.2f}")

    # Category Report 
    print("\n--- CATEGORY REPORT: TOP EARNERS ---")
    top_earners.sort(key=lambda x: x[1], reverse=True)
    for i, (emp_id, pay) in enumerate(top_earners[:3]):
        print(f"{i+1}. Employee ID: {emp_id} - Net Pay: ${pay:.2f}")
        
# menu displaying payroll options
def menu():
    while True:
        print("\n--- BAKER'S COVE PAYROLL ADMIN ---")
        print("Payroll Frequency: MONTHLY") 
        print("1. Load Employees")
        print("2. View Employee List (Filter)")
        print("3. Calculate Payroll (Active Only)")
        print("4. View Payroll Summary Report")
        print("5. Exit")
        
        choice = input("Enter choice: ")
        
        match choice: # Selection statements
            case '1': load_employees()
            case '2': view_employees()
            case '3': calculate_payroll()
            case '4': payroll_reports()
            case '5': break
            case _: print("Invalid selection.")

if __name__ == "__main__":
    menu()