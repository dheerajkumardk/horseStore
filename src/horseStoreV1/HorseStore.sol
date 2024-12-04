// SPDX-License-Identifier: MIT

pragma solidity 0.8.26;

contract HorseStore {
    uint256 numberOfHorses;

    function updateHorseNumber(uint256 newNumberOfHorses) external {
        numberOfHorses = newNumberOfHorses;
    }

    function readNumberOfHorses() external view returns (uint256) {
        return numberOfHorses;
    }
}
// 0xcdfead2e0000000000000000000000000000000000000000000000000000000000000001