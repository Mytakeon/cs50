SELECT movies.title
FROM movies
JOIN stars ON stars.movie_id=movies.id
JOIN people ON people.id=stars.person_id
WHERE people.name IN ("Johnny Depp", "Helena Bonham Carter") 
GROUP BY movies.id
HAVING count (people.id) = 2;