SELECT DISTINCT people.name
FROM movies
JOIN ratings ON ratings.movie_id=movies.id
JOIN directors ON directors.movie_id=movies.id
JOIN people ON people.id=directors.person_id
WHERE ratings.rating >= 9;