
create view VsitelistSensortype as select a.*, b.sid as sid, b.sname as sensorname, b.ename as sensorename, b.ps as sensorps , a.id as mainid from Vsitelist as a left join sensortype as b on a.sensortype = b.sid order by a.siteid asc , a.mac asc ; 



this select Vsitelist with sensortype
select a.*, b.sid as sid, b.sname as sensorname, b.ename as sensorename, b.ps as sensorps , a.id as mainid from Vsitelist as a left join sensortype as b on a.sensortype = b.sid order by a.siteid asc , a.mac asc ; 

create view VWindsitelist as select a.*, b.sid as sid, b.sname as sensorname, b.ename as sensorename, b.ps as sensorps , a.id as mainid from Vsitelist as a left join sensortype as b on a.sensortype = b.sid where a.sensortype = '31' order by a.siteid asc , a.mac asc ; 




select from VsitelistSensortype only for wind
 select a.*, b.sid as sid, b.sname as sensorname, b.ename as sensorename, b.ps as sensorps , a.id as mainid from Vsitelist as a left join sensortype as b on a.sensortype = b.sid where a.sensortype = '31' order by a.siteid asc , a.mac asc ; 

