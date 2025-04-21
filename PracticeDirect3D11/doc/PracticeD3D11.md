# PracticeD3D11

## 삽질 기록

### 2025.04.19

- 기본 값 초기화 오류
    - 증상
        - 아무것도 렌더링 되지 않음
    - 원인
        - 변환 행렬을 위한 Position, Rotation, Scale 값을 잘못된 값으로 초기화
        - Position, Rotation의 경우 Vector4 자료형인데 w값을 0으로 둔채 1로 초기화 하지 않고 사용
        - Scale의 경우 (0,0,0,0)값으로 사용

### 2025.04.20

- 변환 행렬을 HLSL Shader로 넘겨 줄때의 처리 오류
    - 증상
        - Position값을 변경하여 행렬을 계산 하였으나 렌더링 결과는 원치 않게 나옴
        - ![](attachments/Pasted%20image%2020250420150038.png)
    - 원인
        - XMMATRIX를 사용한 변환행렬을 HLSL Shader로 넘겨줄 때는 전치를 해줘서 전달 필요
        - [전치를 해서 넘겨줘야 하는 이유](https://woo-dev.tistory.com/249)
        - XMMATRIX는 행렬을 열 우선으로 저장하고, HLSL은 행렬을 행 우선으로 저장하기 때문
        - ![](Pasted%20image%2020250420151223.png)
