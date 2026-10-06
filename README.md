# Algorithm

알고리즘 문제 풀이를 꾸준히 기록하며 코딩 테스트를 준비하고, 문제 해결 능력을 향상하기 위한 개인 저장소입니다.

## Platforms

- BOJ
- Programmers
- SWEA
- LeetCode


<!-- STATS_START -->
## Solved Problems

### BOJ

| Tier | Problems |
|---|---:|
| Bronze | 14 |
| Silver | 66 |
| Gold | 7 |
| Platinum | 0 |
| **Total** | **87** |

### Programmers

| Level | Problems |
|---|---:|
| Lv0 | 64 |
| Lv1 | 86 |
| Lv2 | 86 |
| Lv3 | 37 |
| Lv4 | 1 |
| Lv5 | 0 |
| **Total** | **274** |

### SWEA

| Difficulty | Problems |
|---|---:|
| D1 | 19 |
| D2 | 14 |
| D3 | 29 |
| D4 | 13 |
| D5 | 3 |
| D6 | 1 |
| Other (Mock / Unclassified) | 10 |
| **Total** | **89** |

### LeetCode

| Difficulty | Problems |
|---|---:|
| Easy | 0 |
| Medium | 10 |
| Hard | 0 |
| **Total** | **10** |

<!-- STATS_END -->

## 문제 수 자동 갱신

처음 클론한 저장소에서는 `git config core.hooksPath .githooks`를 한 번 실행합니다.
이후 커밋할 때 Git에 추가한 풀이 파일을 기준으로 위 표를 다시 계산하고 README를 같은 커밋에 포함합니다.
`main`에 푸시하면 GitHub Actions도 다시 계산하여 누락된 갱신을 보완합니다.

## Languages

- Java
- Python (필요한 경우)

## Organization

문제는 플랫폼별로 분류하고, 각 플랫폼 안에서 난이도별 폴더로 관리합니다.

```text
Algorithm/
├─ BOJ/
│  ├─ Bronze/
│  ├─ Silver/
│  ├─ Gold/
│  └─ Platinum/
├─ Programmers/
│  ├─ Lv1/
│  ├─ Lv2/
│  ├─ Lv3/
│  └─ Lv4/
├─ SWEA/
│  ├─ D2/
│  ├─ D3/
│  ├─ D4/
│  ├─ D5/
│  ├─ D6/
│  └─ Mock/
└─ LeetCode/
   ├─ Easy/
   ├─ Medium/
   └─ Hard/
```

Git은 빈 폴더를 추적하지 않으므로, 난이도 폴더는 첫 문제 풀이를 추가할 때 저장소에 반영됩니다.

## File Naming

가능하면 문제 번호와 문제 이름을 함께 사용합니다. 파일명에 사용할 수 없는 특수문자는 제거하거나 언더스코어(`_`)로 바꿉니다.
문제별 풀이 로직이 아직 없는 파일은 이름 끝에 `_미완성`을 붙여 완성 풀이와 구분합니다.

```text
BOJ/Gold/1753_최단경로.java
Programmers/Lv2/전력망을_둘로_나누기.java
SWEA/D4/5644_무선충전.java
LeetCode/Medium/3_Longest_Substring_Without_Repeating_Characters.java
```

## Principles

- 문제별 소스 코드를 저장합니다.
- 핵심 알고리즘을 파악한 뒤 풀이합니다.
- 필요한 경우 풀이 아이디어와 시간 복잡도를 주석이나 문서로 기록합니다.
