import psycopg2
import json

host = 'localhost'
dbname = 'recipes'
user = 'postgres'
password = 'andresrmlnx15'
port = '5432'

conn = psycopg2.connect(host=host, dbname=dbname, user=user, password=password, port=port)

cur = conn.cursor()

sql = "SELECT * FROM tasks;"

try:
    cur.execute(sql)

    records = cur.fetchall()
    
    col_names = [desc[0] for desc in cur.description]

    data_list = []
    
    for record in records:
        data_dict = dict(zip(col_names, record))
        data_dict['ingredients'] = json.loads(data_dict['ingredients'])
        data_dict['instructions'] = json.loads(data_dict['instructions'])
        data_list.append(data_dict)

    with open('database_dump.json', 'w', encoding='utf-8') as json_file:
        json.dump(data_list, json_file, ensure_ascii=False, indent=4)

    print("JSON dump was created successfully.")
    
except Exception as e:
    print("An error occurred:", e)

finally:
    cur.close()
    conn.close()
