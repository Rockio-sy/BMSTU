import json
import random
import string
from datetime import datetime, timedelta
import time

# Function to generate random data for the students table
def generate_student_record():
    first_names = ["Alice", "Bob", "Charlie", "Diana", "Eve"]
    last_names = ["Smith", "Johnson", "Williams", "Brown", "Jones"]
    
    record = {
        "iasdasd": random.randint(1, 100000),
        "first_name": random.choice(first_names),
        "last_name": random.choice(last_names),
        "study_year": random.randint(1, 5),  # Assuming students are in year 1 to 5
        "class_group": ''.join(random.choices(string.ascii_uppercase, k=2)) + str(random.randint(1, 10)),  # Example: "A3"
        "created_at": (datetime.now() - timedelta(days=random.randint(0, 365))).strftime("%Y-%m-%d %H:%M:%S"),
        "updated_at": datetime.now().strftime("%Y-%m-%d %H:%M:%S"),
        "age": random.randint(18, 25),  # Assuming typical student ages
    }
    return record

# Function to generate a file name
def generate_filename():
    timestamp = datetime.now().strftime("%Y%m%d_%H%M%S")
    unique_id = random.randint(1000, 9999)  # Random unique identifier
    return f"file_{unique_id}_students_{timestamp}.json"

# Main loop to generate files every 5 minutes
output_directory = "./nifi/in_file"  # Change this to your desired directory

# Ensure the directory exists
import os
os.makedirs(output_directory, exist_ok=True)

while True:
    # Generate data for 10 students (example batch size)
    data = [generate_student_record() for _ in range(10)]
    
    # Create the filename
    filename = os.path.join(output_directory, generate_filename())
    
    # Write the data to a JSON file
    with open(filename, "w") as f:
        json.dump(data, f, indent=4)
    
    print(f"Generated file: {filename}")
    
    # Wait 5 minutes before generating the next file
    time.sleep(5)  # 300 seconds = 5 minutes

