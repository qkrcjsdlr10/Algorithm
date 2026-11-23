from algo import hashing, layout


def test_whitespace_inside_lines_is_not_normalized():
    a = b'String a = "hello world";\n'
    b = b'String a = "helloworld";\n'
    assert hashing.normalized_hash(a) != hashing.normalized_hash(b)
    assert hashing.match_kind(a, hashing.raw_hash(b), hashing.normalized_hash(b)) is None


def test_tabs_and_trailing_spaces_are_significant():
    a = b"R\"(x  \n)\";\n"
    b = b"R\"(x\n)\";\n"
    assert hashing.normalized_hash(a) != hashing.normalized_hash(b)
    assert hashing.normalized_hash(b"\tint x;\n") != hashing.normalized_hash(b"    int x;\n")


def test_line_endings_bom_and_final_newline_are_normalized():
    site = b"class A {\n}\n"
    local = b"\xef\xbb\xbfclass A {\r\n}\r\n\r\n"
    assert hashing.match_kind(site, hashing.raw_hash(site)) == "raw"
    assert hashing.match_kind(local, hashing.raw_hash(site), hashing.normalized_hash(site)) == "normalized"
    # site known only by raw hash (metadata.json)
    assert hashing.match_kind(local, hashing.raw_hash(site)) == "normalized"
    assert hashing.match_kind(b"class A {\r\n}", hashing.raw_hash(b"class A {\r\n}\r\n")) == "normalized"


def test_parse_paths():
    p, _ = layout.parse_path("BOJ/Silver/1000_A+B_CPP.cpp")
    assert (p.site, p.tier, p.problem_id, p.lang_key) == ("BOJ", "Silver", "1000", "cpp")
    p, _ = layout.parse_path("Programmers/Lv2/169199_리코쳇_로봇_Java.java")
    assert (p.problem_id, p.language) == ("169199", "Java")
    p, _ = layout.parse_path("LeetCode/SQL/Hard/185_Department_Top_Three_Salaries.sql")
    assert p.is_sql and p.tier == "Hard"
    p, _ = layout.parse_path("Programmers/Lv3/43162_네트워크_미완성_Java.java")
    assert p.incomplete
    assert layout.parse_path("CodeTree/trail4/빙하/glacier.java")[0] is None
    assert layout.parse_path("BOJ/Silver/별찍기.cpp")[0] is None          # no number: never guessed
    assert layout.parse_path("BOJ/Diamond/1_x_CPP.cpp")[0] is None       # not counted by README
    assert layout.parse_path("scratch/1000.java")[0] is None


def test_build_paths():
    assert layout.build_path("Programmers", "Lv1", "59034", "모든 레코드 조회하기", "MySQL", ".sql") == \
        "Programmers/SQL/Lv1/59034_모든_레코드_조회하기_MySQL.sql"
    assert layout.build_path("SWEA", "D9", "1", "x", "Java", ".java") == "SWEA/Unclassified/1_x_Java.java"
    assert layout.build_path("LeetCode", "Medium", "3", "Longest: Sub?", "CPP", ".cpp") == \
        "LeetCode/Medium/3_Longest_Sub_CPP.cpp"
    # every built path parses back to the same identity
    for path in ("Programmers/SQL/Lv1/59034_a_MySQL.sql", "SWEA/Unclassified/1_x_Java.java"):
        assert layout.parse_path(path)[0] is not None
