-- DONE BY DOCKER
-- CREATE USER IF NOT EXISTS postgres;
-- CREATE DATABASE IF NOT EXISTS postgres;
-- GRANT ALL PRIVILEGES ON DATABASE postgres TO postgres;

\connect postgres;

CREATE EXTENSION IF NOT EXISTS "uuid-ossp";

CREATE TABLE public.talent (
	id uuid NOT NULL DEFAULT uuid_generate_v4(),
	first_name varchar(40),
	middle_name varchar(40),
	last_name varchar(40),
	email varchar(100),
	created timestamp NOT NULL DEFAULT CURRENT_TIMESTAMP,
	updated timestamp,
	CONSTRAINT talent_id_pk PRIMARY KEY (id)
);

INSERT INTO public.talent (
    first_name,
    middle_name,
    last_name,
    email
)
VALUES
(
    'John',
    'Doe',
    'Smith',
    'john.smith@talent.com'
),
(
    'Jane',
    'Katalina',
    'Smith',
    'jane.smith@talent.com'
);
