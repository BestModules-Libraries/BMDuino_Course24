
create view Vsite as select a.*, b.areaname from site as a left join area as b on a.areaid = b.areaid  ;



select a.*, b.areaname from site as a left join area as b on a.areaid = b.areaid  ;