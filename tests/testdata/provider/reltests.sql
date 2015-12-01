-- Table: qgis_test.authors

-- DROP TABLE qgis_test.authors;

CREATE TABLE qgis_test.authors
(
  pk serial NOT NULL,
  name character varying(255),
  CONSTRAINT authors_pkey PRIMARY KEY (pk),
  CONSTRAINT authors_name_key UNIQUE (name)
);

-- Table: qgis_test.books

-- DROP TABLE qgis_test.books;

CREATE TABLE qgis_test.books
(
  pk serial NOT NULL,
  name character varying(255),
  CONSTRAINT books_pkey PRIMARY KEY (pk),
  CONSTRAINT books_name_key UNIQUE (name)
);

-- Table: qgis_test.books_authors

-- DROP TABLE qgis_test.books_authors;

CREATE TABLE qgis_test.books_authors
(
  fk_book integer NOT NULL,
  fk_author integer NOT NULL,
  CONSTRAINT books_authors_pkey PRIMARY KEY (fk_book, fk_author),
  CONSTRAINT books_authors_fk_author_fkey FOREIGN KEY (fk_author)
      REFERENCES qgis_test.authors (pk) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE CASCADE,
  CONSTRAINT books_authors_fk_book_fkey FOREIGN KEY (fk_book)
      REFERENCES qgis_test.books (pk) MATCH SIMPLE
      ON UPDATE NO ACTION ON DELETE CASCADE
);
