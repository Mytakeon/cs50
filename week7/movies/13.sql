SELECT DISTINCT people.name
FROM movies
JOIN stars ON stars.movie_id=movie_id
JOIN people ON people.id=stars.person_id
WHERE people.name != "Kevin Bacon"  -- Don't include Kevin in the list
AND 
stars.movie_id IN (SELECT movies.id  -- List of the movies Kevin stared in
	FROM movies
	JOIN stars ON stars.movie_id=movies.id
	JOIN people ON people.id=stars.person_id
	WHERE people.name= "Kevin Bacon" AND people.birth=1958)
