LIBRARY IEEE;
USE IEEE.STD_LOGIC_1164.ALL;

ENTITY INT IS
PORT (
  IREN : IN  STD_LOGIC; -- K4.0
  EINT : IN  STD_LOGIC; -- K4.1
  ICEN : OUT STD_LOGIC; -- A2
  ACK  : OUT STD_LOGIC; -- A1
  REQ  : OUT STD_LOGIC; -- A0
  RST  : IN  STD_LOGIC; -- K4.7
  I_REQ: IN  STD_LOGIC; -- INT
  CLK  : IN  STD_LOGIC  -- CLOCK
);
END INT;

ARCHITECTURE behv OF INT IS

SIGNAL R_REQ, R_ACK: STD_LOGIC;
SIGNAL FATCH_INT: STD_LOGIC;

BEGIN

  REQ  <= R_REQ;
  ACK  <= R_ACK;
  ICEN <= FATCH_INT;

  PROCESS(I_REQ, RST, EINT)
  BEGIN
    IF (EINT = '0') OR (RST = '1') THEN
      R_REQ <= '0';
    ELSIF I_REQ'EVENT AND I_REQ = '1' THEN
      R_REQ <= '1';
    END IF;
  END PROCESS;

  PROCESS(CLK, RST, EINT, IREN, R_REQ, R_ACK, FATCH_INT)
  BEGIN
    IF (IREN = '0') AND (R_REQ = '1') AND (R_ACK = '0') THEN
      FATCH_INT <= '0';
    ELSE
      FATCH_INT <= '1';
    END IF;

    IF (EINT = '0') OR (RST = '1') THEN
      R_ACK <= '0';
    ELSIF CLK'EVENT AND CLK = '1' THEN
      IF FATCH_INT = '0' THEN
        R_ACK <= R_REQ;
      END IF;
    END IF;
  END PROCESS;

END behv;

