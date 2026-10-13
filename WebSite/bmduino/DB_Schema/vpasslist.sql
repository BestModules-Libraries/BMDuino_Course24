view

create view vpasslist as  select a.* , b.rfidkey , b.activate as ractivate , b.startdatetime as rstartdatetime , b.enddatetime as renddatetime from passlist as a left join rfidlist as b on a.mid = b.id where 1 


select a.* , b.rfidkey , b.activate as ractivate , b.startdatetime as rstartdatetime , b.enddatetime as renddatetime from passlist as a left join rfidlist as b on a.mid = b.id where 1 


