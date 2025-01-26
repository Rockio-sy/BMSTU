/* Take the Data as JSON*/
select to_jsonb(row_to_json(teachers)) as "Result" from teachers ; 

/* Import JSON file into PG, */
/*From the pre command we can ge the data to temp file in /tmp/task.json becaues Postgres has access by default to that directory, then we can move the file anywhere we want
 * Command to exe "\COPY (SELECT to_jsonb(row_to_json(books)) FROM  books) TO '/tmp/task1.json' "
 * After exe, we can use the file to do second task
 */
create table if not exists json_data(
id SERIAL primary key,
data JSONB);

/*If they want to create the same table, u can use CREATE table_name AS SELECT * FROM original_table WHERE 1=2;*/
/*Use it in terminal*/
/*\copy json_data(data) from '/tmp/task_in.json'*/

select * from json_data ;


/* Third task*/
create table third_task(id SERIAL primary key, data JSONB);

insert into third_task (data)
values
('{"Name": "Max", "size":["M", "XL"]}');

select * from third_task;

/*Fourth task*/

select data -> 'Name' as  Name from third_task ; /* Get only the name*/
select data ->> 'size'from third_task ; /*get array*/
select data -> 'size' ->> 0 from third_task ; /* Get first element of the array*/
select data ? 'size' as "Size" from third_task where id=1; /*Check if size exists in the data JSON --> if V then yes, if empty, then no*/
select data -> 'size' ? 'S' as "Small Size" from third_task where id = 2; /*Check if the user with id 2 has S size*/

select data as D from third_task ;

/*Add to the size array new value in spec index */
update third_task set data=jsonb_insert(data, '{size, 1}', '"tarekL"', true) where id = 4; 

/*Delete from the JSON documnet*/
UPDATE third_task set data=jsonb_set(data, '{size}', (data -> 'size')-1) WHERE id = 4;

/*Add new Key with value*/
update third_task set data=jsonb_insert(data, '{age}', '24'::JSONB) WHERE id=2;

/*Delete a key*/
update third_task set data = data -'age' WHERE id=2 ;


/*Split to many rows --> It works only with arrays*/
SELECT id, jsonb_array_elements(data -> 'size') AS "Size" FROM third_task;





