/* Scalar Function */ 
CREATE OR REPLACE FUNCTION year_of_birth(student_id INT)
RETURNS INT AS $$
    import datetime
    current_year = datetime.datetime.now().year

    query = """
    SELECT age
    FROM students
    WHERE id = %s
    """ % student_id

    result = plpy.execute(query)


    if result and result[0]['age'] is not None:
        age = result[0]['age']
        year_of_birth = current_year - age
        return year_of_birth
    else:
        return None
$$ LANGUAGE plpython3u;
select age from students s where id = 3;
select * from year_of_birth (3);


/* Aggregate function */
create or replace function
get_avg_age_in_class(class_id INT)
returns FLOAT as $$
	query = """
	select age from students s 
	join stud_class sc on s.id = sc.student_id 
	join classes c on sc.class_id = c.id 
	where c.id = %s;
	"""% class_id
	
	result = plpy.execute(query)
	
	if not result:
		return None
	
	total_age = sum(row['age'] for row in result)
	total_students = len(result)
	
	if total_students > 0:
		return total_age / total_students
	else:
		return None
$$ language plpython3u;
select * from get_avg_age_in_class(3);


/* Table function*/
create or replace function 
get_students_in_class(class_id INT)
returns TABLE(first_name VARCHAR, last_name VARCHAR, age INT)
as $$
	query = """
	SELECT first_name, last_name, age 
	FROM students s
	JOIN stud_class sc ON s.id = sc.student_id
	JOIN classes c ON sc.class_id = c.id
	WHERE c.id = %s
	""" % class_id
	
	result = plpy.execute(query)
	
	for row in result:
		return result
$$ language plpython3u;
select * from get_students_in_class(3);


/* Procedure function*/
create or replace procedure update_student_age(student_id INT, increment_year INT)
language plpython3u as $$
	query = """
	UPDATE students s
	SET age = age + %s
	WHERE id = %s
	""" % (increment_year, student_id)
	
	result = plpy.execute(query)
	plpy.notice('Student with id %s is %s Y.O now.' %(student_id, increment_year))
$$;
select id, age from students s where id = 1;
call update_student_age(1, 1);
select id, age from students s where id = 1;


/* Trigger */
/* create a table to save the logs*/
CREATE TABLE IF NOT EXISTS students_audit (
    change_id SERIAL PRIMARY KEY,
    student_id INT,
    old_first_name VARCHAR(50),
    new_first_name VARCHAR(50),
    old_last_name VARCHAR(50),
    new_last_name VARCHAR(50),
    old_study_year INT,
    new_study_year INT,
    old_class_group VARCHAR(20),
    new_class_group VARCHAR(20),
    old_age INT,
    new_age INT,
    change_timestamp TIMESTAMP DEFAULT CURRENT_TIMESTAMP
);



DROP FUNCTION IF EXISTS log_student_changes;

CREATE OR REPLACE FUNCTION log_student_changes()
RETURNS TRIGGER AS $$
BEGIN
    -- Only log if any of the tracked fields have changed
    IF NEW.first_name != OLD.first_name OR
       NEW.last_name != OLD.last_name OR
       NEW.study_year != OLD.study_year OR
       NEW.class_group != OLD.class_group OR
       NEW.age != OLD.age THEN

        -- Insert the change record into the students_audit table
        INSERT INTO students_audit (student_id, old_first_name, new_first_name,
                                    old_last_name, new_last_name,
                                    old_study_year, new_study_year,
                                    old_class_group, new_class_group,
                                    old_age, new_age)
        VALUES (OLD.id,
                OLD.first_name, NEW.first_name,
                OLD.last_name, NEW.last_name,
                OLD.study_year, NEW.study_year,
                OLD.class_group, NEW.class_group,
                OLD.age, NEW.age);
    END IF;
    RETURN NEW;
END;
$$ LANGUAGE plpgsql;

CREATE TRIGGER trigger_log_student_changes
AFTER UPDATE ON students
FOR EACH ROW
EXECUTE FUNCTION log_student_changes();

/* User defined data type*/
create type student_info as (
	first_name VARCHAR(50),
	last_name VARCHAR(50),
	study_year INT,
	class_group VARCHAR(20),
	age INT
);

create or replace function get_student_info(student_id INT)
returns student_info as $$
	import plpy
	
	query = f"SELECT first_name, last_name, study_year, class_group, age FROM students WHERE id = {student_id};"
	result = plpy.execute(query)
	
	if len(result) == 0:
		return None
	
	first_name = result[0]['first_name']
	last_name = result[0]['last_name']
	study_year = result[0]['study_year']
	class_group = result[0]['class_group']
	age = result[0]['age']
		
	return (first_name, last_name, study_year, class_group, age)
$$ language plpython3u;


/*Use it in the Terminal to get clear output of the function as a tuple*/
select get_student_info(4) as studetn;



