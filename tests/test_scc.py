#!/usr/bin/env python3
"""
Unit tests for Sequence Certainty Coefficient (SCC) calculations.
"""
import math
import sys
from pathlib import Path

# Add src to path for imports
sys.path.insert(0, str(Path(__file__).parent.parent / "src"))

import pytest
from scc import solexa2phred, phred2errprob, multiply_quals


class TestSolexa2Phred:
    """Tests for Solexa to Phred quality score conversion."""

    def test_high_quality_score(self) -> None:
        """Test conversion of high quality Solexa score."""
        result = solexa2phred(40.0)
        # At high quality, Solexa ≈ Phred
        assert result == pytest.approx(40.0, rel=0.01)

    def test_medium_quality_score(self) -> None:
        """Test conversion of medium quality Solexa score."""
        result = solexa2phred(20.0)
        expected = 10.0 * math.log10(1.0 + 10.0 ** (20.0 / 10.0))
        assert result == pytest.approx(expected, rel=0.001)

    def test_low_quality_score(self) -> None:
        """Test conversion of low quality Solexa score."""
        result = solexa2phred(3.0)
        assert result > 0.0
        assert result < 10.0


class TestPhred2ErrProb:
    """Tests for Phred to error probability conversion."""

    def test_perfect_quality(self) -> None:
        """Test error probability for Q40 (perfect quality)."""
        result = phred2errprob(40.0)
        assert result == pytest.approx(0.0001, rel=0.01)

    def test_good_quality(self) -> None:
        """Test error probability for Q30 (good quality)."""
        result = phred2errprob(30.0)
        assert result == pytest.approx(0.001, rel=0.01)

    def test_poor_quality(self) -> None:
        """Test error probability for Q10 (poor quality)."""
        result = phred2errprob(10.0)
        assert result == pytest.approx(0.1, rel=0.01)

    def test_probability_range(self) -> None:
        """Ensure error probability is between 0 and 1."""
        for q in [0, 10, 20, 30, 40]:
            prob = phred2errprob(float(q))
            assert 0.0 <= prob <= 1.0


class TestMultiplyQuals:
    """Tests for quality score multiplication (combined error probability)."""

    def test_single_quality_score(self) -> None:
        """Test with single quality score."""
        # Single Q40 score
        result = multiply_quals("40")
        phred = solexa2phred(40.0)
        err_prob = phred2errprob(phred)
        pci = 1.0 - err_prob
        expected = 1.0 - pci
        assert result == pytest.approx(expected, rel=0.01)

    def test_uniform_high_quality(self) -> None:
        """Test with uniform high quality scores."""
        # Three Q40 scores
        result = multiply_quals("40 40 40")
        assert result < 0.001  # Very low error probability

    def test_mixed_quality_scores(self) -> None:
        """Test with mixed quality scores."""
        result = multiply_quals("40 30 20")
        assert 0.0 < result < 1.0

    def test_result_increases_with_length(self) -> None:
        """Longer sequences have higher combined error probability."""
        short = multiply_quals("40 40")
        long = multiply_quals("40 40 40 40")
        assert long > short


if __name__ == "__main__":
    pytest.main([__file__, "-v"])
