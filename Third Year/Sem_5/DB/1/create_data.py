from faker import Faker
import psycopg2

# Initialize Faker
fake = Faker()

# Connect to PostgreSQL
conn = psycopg2.connect(
    dbname='univ',
    user='postgres',
    password='postgres',
    host='localhost'
)
cursor = conn.cursor()

# Generate and insert data for teachers
teachers = []
for _ in range(1000):
    teachers.append((
        fake.first_name()[:50],
        fake.last_name()[:50],
        fake.job()[:50]  # Assuming subject can be a job title
    ))

cursor.executemany("""
    INSERT INTO teachers (first_name, last_name, subject, created_at, updated_at)
    VALUES (%s, %s, %s, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)
""", teachers)

# Commit to get teacher IDs
conn.commit()

# Retrieve teacher IDs
cursor.execute("SELECT id FROM teachers")
teacher_ids = [row[0] for row in cursor.fetchall()]

# Generate and insert data for classes
classes = []
for _ in range(1000):
    classes.append((
        fake.word()[:50],  # Subject
        fake.word()[:10],  # Room number
        fake.random_element(elements=teacher_ids)  # Random teacher_id
    ))

cursor.executemany("""
    INSERT INTO classes (subject, room_number, teacher_id, created_at, updated_at)
    VALUES (%s, %s, %s, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)
""", classes)

# Commit to get class IDs
conn.commit()

# Retrieve class IDs
cursor.execute("SELECT id FROM classes")
class_ids = [row[0] for row in cursor.fetchall()]

# Generate and insert data for students
students = []
for _ in range(1000):
    students.append((
        fake.first_name()[:50],
        fake.last_name()[:50],
        fake.random_int(min=1, max=4),  # Study year
        fake.word()[:20]  # Class group
    ))

cursor.executemany("""
    INSERT INTO students (first_name, last_name, study_year, class_group, created_at, updated_at)
    VALUES (%s, %s, %s, %s, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)
""", students)

# Commit to get student IDs
conn.commit()

# Retrieve student IDs
cursor.execute("SELECT id FROM students")
student_ids = [row[0] for row in cursor.fetchall()]

# Generate and insert data for stud_class
stud_class = []
for _ in range(1000):
    stud_class.append((
        fake.random_element(elements=student_ids),  # Random student_id
        fake.random_element(elements=class_ids)  # Random class_id
    ))

cursor.executemany("""
    INSERT INTO stud_class (student_id, class_id, created_at, updated_at)
    VALUES (%s, %s, CURRENT_TIMESTAMP, CURRENT_TIMESTAMP)
""", stud_class)

# Commit and close connection
conn.commit()
cursor.close()
conn.close()

