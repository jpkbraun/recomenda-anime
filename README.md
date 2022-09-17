# recomenda-anime
// PT-BR

Programa feito para manipulação de banco de dados em C++. Feito por Arthur Alves em colaboração com João Pedro Braun durante o período 2022/1 da UFRGS para o trabalho final de Classificação e Pesquisa de Dados, sob a tutela do professor Renan Maffei.

O programa utiliza de CSVs retirados do site MyAnimeList para a criação de um banco de dados armazenado em arquivos binários, de forma em que os dados persistem. São utilizadas árvores B+ para acesso de chaves inteiras e árvores TRIE para acesso de chaves em string. Também são feitos arquivos invertidos para o acesso de registros através de campos secundários.

O programa contém as seguintes funções: recomendaAnime, buscaAnime, excluiAnime, ordenaAnime, ordenaAnimeInverso, buscaAnimePrefixo, buscaAnimeGenero, buscaAnimeEstudio, buscaAnimeGeneroLicenciador e buscaAnimeDoisEstudios.

// EN-US

Program made for database manipulation in C++. Made by Arthur Alves in collab with João Pedro Braun during UFRGS' 2022/1 period for the final Classification and Search of Data assignment, under professor Renan Maffei's tutelage.

The programa uses CSVs taken from MyAnimeList site for the creation of a database stored in binary files, in a way data persists. B+ trees are used to access integer keys and TRIE trees are used to access string keys. There is also inverted files used for data access through secondary attributes.

The program contains the following funcions: recommendAnime, searchAnime, deleteAnime, sortAnime, sortAnimeInverse, searchAnimePrefix, searchAnimeGenre, searchAnimeStudios, searchAnimeGenreLicensor and searchAnimeTwoStudios.
