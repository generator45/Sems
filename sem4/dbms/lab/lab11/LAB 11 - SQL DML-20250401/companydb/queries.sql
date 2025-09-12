select salary as 'monthly salary' from employee;

update employee set salary = null where dno = 1;

select avg(salary) from employee;

select dno, avg(salary) from employee group by dno;

select dno, count(fname), sum(salary) from employee where sex='F' group by dno;

select dno,fname,  count(fname),sum(salary) from employee where sex='F' group by dno;
-- this doen't work because fname is not in group by clause

select dno,count(fname),avg(salary) from employee group by dno having avg(salary) > 10;
-- having clause is used to filter the results of a group by clause

SELECT dno, COUNT(fname) AS employee_count, AVG(salary) AS avg_salary
FROM employee
WHERE sex = 'F' -- Filters rows before grouping
GROUP BY dno
HAVING AVG(salary) > 10000; 


select fname, salary from employee where salary = (select max(salary) from employee);
-- this will return all employees with the max salary
select fname, salary from employee where salary = max(salary);
-- this doesn't work because max(salary) is a aggregate function and salary is a 


select e1.fname, e1.dno, e1.salary
from employee as e1
where e1.salary >= (select avg(e2.salary) 
                    from employee as e2 
                    where e2.dno = e1.dno);
-- corelated subquery

select e1.dno, count(e1.ssn) 
from employee as e1
group by dno having count(ssn) = (select count(e2.ssn) 
                                  from employee as e2
                                  where e2.dno = e1.dno);


-- CTEs

with dept_counts as (
    select dno, count(ssn) as dept_count
    from employee
    group by dno
)

select d1.dno, d1.dept_count from dept_counts as d1
where dept_count = (select max(d2.dept_count) from dept_counts as d2);

----- JOINS

select fname, dno, dname, salary
from employee, department
where employee.dno = department.dnumber;

select fname, dno , dname, salary
from employee
inner join department on dno = dnumber;
-- this is the same as the above query

select fname, dno , dname, pno, hours
from employee as e inner join department as d on e.dno = d.dnumber
inner join works_on as w on e.ssn = w.essn; 

select fname, dno , dname, pno, pname hours
from employee as e inner join department as d on e.dno = d.dnumber
inner join works_on as w on e.ssn = w.essn
inner join project as p on w.pno = p.pnumber;


-- Outer Join
select e.ssn, e.fname, d.dependent_name, d.relationship
from employee as e inner join dependent d on e.ssn = d.essn;

select ssn from employee where ssn not in 
(select essn from dependent);
-- this will return all employees that have no dependents


select fname, dno , dname, pno, hours
from employee as e left join department as d on e.dno = d.dnumber
inner join works_on as w on e.ssn = w.essn; 

select e.ssn, e.fname, d.dependent_name, d.relationship
from employee as e left join dependent d on e.ssn = d.essn;


select e.ssn, e.fname, d.dependent_name, d.relationship from dependent as d right outer join employee e on d.essn = e.ssn;
-- swapping the order of the tables in a right outer join will give you the same result as a left outer join