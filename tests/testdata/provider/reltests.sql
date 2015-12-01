DROP TABLE "qgis_test"."child";
DROP TABLE "qgis_test"."parent";
DROP SEQUENCE qgis_test.parent_id_seq;
DROP SEQUENCE qgis_test.child_id_seq;

CREATE SEQUENCE qgis_test.parent_id_seq
  INCREMENT 1
  MINVALUE 1
  MAXVALUE 9223372036854775807
  START 1
  CACHE 1;


CREATE SEQUENCE qgis_test.child_id_seq
  INCREMENT 1
  MINVALUE 1
  MAXVALUE 9223372036854775807
  START 1
  CACHE 1;

CREATE TABLE "qgis_test"."parent" (
    "pk" integer PRIMARY KEY DEFAULT nextval('qgis_test.parent_id_seq'),
    "name" CHARACTER VARYING( 255 ) COLLATE "pg_catalog"."default" UNIQUE
);

CREATE TABLE "qgis_test"."child" (
    "pk" integer PRIMARY KEY DEFAULT nextval('qgis_test.child_id_seq'),
    "name" CHARACTER VARYING( 255 ) COLLATE "pg_catalog"."default" UNIQUE,
    "fk" integer,
    FOREIGN KEY (fk) REFERENCES qgis_test.parent(pk)
);
