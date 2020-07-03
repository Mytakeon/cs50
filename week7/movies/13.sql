WITH joined AS (SELECT stars.movie_id, people.name, people.birth
	FROM movies
	JOIN stars ON stars.movie_id=movies.id
	JOIN people ON people.id=stars.person_id),

movies_with_kevin AS (SELECT movie_id
	FROM joined
	WHERE name= "Kevin Bacon" AND birth=1958)
    
SELECT DISTINCT name
FROM joined
WHERE name != "Kevin Bacon"  -- Don't include Kevin in the list
AND movie_id IN movies_with_kevin
