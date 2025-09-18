# ***hairshop***
<br/>
지정된 미용실과 소비자를 연결하여 예약과 관리를 할 수 있는 프로그램입니다.
<br/>
GUI를 통해 클릭으로 예약과 관리를 진행하며, 
<br/>
비트맵 이미지를 이용하여 이미지를 처리를 하였고,
<br/>
언어는 C언어를 사용하였으며 파일을 사용하여 데이터를 처리하였습니다.
<br/>
중점적으로 프로그램이 구동하는 정확성과 사용자가 예약하는데에 있어서 이미지처리와 파일처리를 집중적으로 할 수 있도록 하였습니다.
<br/>

# **프로젝트의 전체적인 구조**
<br/>
<div style="display: flex; align-items: center;">
    <img src="https://github.com/jwgarde/hair_shop_project/assets/113418319/a53c1ed6-f755-43f1-b61a-99e36aa0e218" width="50%" style="max-width: 500px; border: 1px solid #ddd;">
</div>
<br/>
● 데이터 처리 : 총 5개의 텍스트 파일(회원정보, 디자이너 정보, 헤어스타일 정보, 예약,리뷰)을 사용하여 데이터 처리.
<br/>
<br/>


# **주요 기능 및 진척도 (Main Features & Progress)**

<br/>

<div style="display: flex; align-items: center;">
    <img src="https://github.com/jwgarde/hair_shop_project/assets/113418319/9c3367fd-804f-4d80-a8ea-75bb93b232a3" width="70%" style="max-width: 800px; border: 1.5px solid #ddd;">
</div>
<hr/>
<br/>
<br/>
<br/>
<br/>

# **구현결과**
**●회원 화면 프로토타입**
<br/>
<br/>
<div style="display: flex; align-items: center;">
     <img src="https://github.com/jwgarde/hair_shop_project/assets/113418319/06783c19-2e9e-45be-ae93-7d880782d591" width="80%" style="max-width: 800px; border: 1.5px solid #ddd;">
</div>
<br/>
<br/>
<br/>


**●디자이너 화면 프로토타입**
 <br/>
 <br/>
<div style="display: flex; align-items: center;">
     <img src="https://github.com/jwgarde/hair_shop_project/assets/113418319/213ae1a4-c656-4704-8f6b-ea77ac1ea6cd" width="80%" style="max-width: 800px; border: 1.5px solid #ddd;">
</div>
<br/>
<br/>
<br/>

**●관리자 화면 프로토타입**
  <br/>
  <br/>
 <div style="display: flex; align-items: center;">
     <img src="https://github.com/jwgarde/hair_shop_project/assets/113418319/b5c3af20-4211-441c-9268-532a2e1bd1de" width="80%" style="max-width: 800px; border: 1.5px solid #ddd;">
 </div>
 <br/>
 <br/>
 <br/>

# **사용 언어 및 환경**
<br/>
<div style="display: flex; align-items: center;">
    <img src="https://github.com/jwgarde/hair_shop_project/assets/113418319/1661cd72-9e5a-4c00-86ee-8c777e44b58e" width="50%" style="max-width: 500px; border: 1px solid #ddd;">
</div>
<br/>

# **프로젝트 느낀점 및 향후 계획(개선점)**

__공통사항__
- **<span style="font-size:larger;">성능 개선 및 DB사용 및 JAVA로 재구현</span>**
    - 데이터베이스 인덱싱 및 쿼리 튜닝
    - 자바 코드의 효율적인 활용
  <br/>
- **지저분한 코드 및 간결화 시킬 수 있는 코드에 대한 리팩토링**
    - 성능 개선을 위해 최적화된 알고리즘 사용 
  <br/>
- **개발 중 발생한 어려움 또는 한계점**
    - **프로필 이미지 변경**
        - 사용자가 프로필 관리에서 새 이미지 경로를 입력하면, 해당 이미지를 프로젝트 내부의 지정된 경로로 복사하는 코드(파일 복사               (copy)) 함수 구현.

    - **커서 이동 및 백스페이스(글자 삭제) 기능**
        - 한 줄에 최대 입력 가능한 글자 수와 최대 x, y 좌표값을 설정하여, 입력한 글자 수가 제한을 초과하거나 좌표값을 벗어날 경우 커서가 자동으로 아래로 이동하면서 입력.
        - 백스페이스 기능은 공백을 활용하여 한 글자씩 삭제되는 것을 구현합니다. 한 줄의 최소 글자 수를 초과하지 않으면 커서가 자동으로 위로 이동하여 글자가 삭제.

    - **달력 구현**
        - 배열을 이용하여 어떤 월이든 최대 일 수가 31일이기 때문에 배열 크기를 그 만큼 지정하여, 그 달에 유효하지 않는 일이나 예약이 꽉 차있거나 이미 지난 일이면 0을 해당 인덱스에 저장하고, 유효한 경우에는 그 일자를 해당 인덱스에 저장.
  <br/>
- **피드백 및 개선할 점**

    - **백스페이스 구현**
        - 한글을 최대 글자로 입력 상태에서 공백과 한글이 이어서 작성되어 있을 때 스페이스바를 두번 누르고 백스페이스를 누르면 가끔씩 '?'가 출력되는 현상이 있습니다. 바이트 수와 인덱스를 계산하여 해결을 시도하여 어느정도는 해결 하였으나, 완벽하게 해결을 하지 못했습니다.

    - **미용실 예약 제한**
        - 현재는 한 미용실에 대해서만 예약을 할 수 있도록 프로그램이 개발되어 있습니다. 다중 미용실에 대한 예약 기능을 추가하는 것이 필요합니다.

    - **디자이너 등록 제한**
        - 디자이너는 최대 3명까지 등록할 수 있습니다. 그러나 더 많은 디자이너를 등록할 수 있는 기능이 필요하며, C 언어로 비트맵 이미지로만 디자이너를 표현하는 것에 한계가 있었습니다.

    <br/>



 
