create view Vsitelist as 
SELECT a.*, b.areaid,b.areaname,b.siteid,b.sitename,b.address,b.longitude,b.latitude
FROM sitelist as a left join Vsite as b
on a.Did = b.id

SELECT a.*, b.areaid,b.areaname,b.siteid,b.sitename,b.address,b.longitude,b.latitude
FROM sitelist as a left join Vsite as b
on a.Did = b.id

